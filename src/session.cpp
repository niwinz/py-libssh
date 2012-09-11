#include <cstdio>
#include "common.hpp"
#include "session.hpp"
#include "channel.hpp"
#include "result.hpp"


Session::Session(const std::string &hostname, const int &port) {
    this->hostname = hostname;
    this->port = port;
}

Session::~Session() {
    if (this->c_session != NULL) {
        std::cout << "Destroying session" << std::endl;
    }
}

void Session::start_session() {
    if (this->c_session != NULL) {
        this->disconnect();
    }

    this->c_session = ssh_new();
    if (this->c_session == NULL) {
        throw std::runtime_error("Cannot start session");
    }

    ssh_options_set(this->c_session, SSH_OPTIONS_HOST, this->hostname.c_str());
    ssh_options_set(this->c_session, SSH_OPTIONS_PORT, &this->port);
}


void Session::set_auth_data(const std::string &username, const std::string &password) {
    this->username = username;
    this->password = password;
    this->password_auth = true;

    if (this->c_session != NULL) {
        throw std::runtime_error("Cannot set auth data on session is initialized");
    }

    ssh_options_set(this->c_session, SSH_OPTIONS_USER, this->username.c_str());
}

void Session::connect() {
    this->start_session();

    int rc = ssh_connect(this->c_session);
    if (rc != SSH_OK) {
        fprintf(stderr, "Error connecting to localhost: %s\n", ssh_get_error(this->c_session));
        throw std::runtime_error("Cannot connect to server");
    }

    if (this->password_auth) {
        rc = ssh_userauth_password(this->c_session, NULL, this->password.c_str());
        if (rc != SSH_AUTH_SUCCESS) {
            throw std::runtime_error("Cannot authenticate with server");
        }
    } else {
        rc = ssh_userauth_autopubkey(this->c_session, NULL);
        if (rc != SSH_AUTH_SUCCESS) {
            throw std::runtime_error("Cannot authenticate with server");
        }
    }
}

void Session::disconnect() {
    ssh_disconnect(this->c_session);
    ssh_free(this->c_session);
}

ssh_session Session::get_c_session() {
    return this->c_session;
}

/*
 * Standard method for executing small comamands.
*/

execresultptr Session::execute(const std::string &command) {
    Channel *channel = new Channel(this);

    int rc = ssh_channel_request_exec(channel->get_c_channel(), command.c_str());
    if (rc != SSH_OK) {
        fprintf(stderr, "Error connecting to localhost: %s\n", ssh_get_error(this->c_session));
        throw std::runtime_error("Cannot execute command");
    }

    return execresultptr(new ExecResult(channel));
}
