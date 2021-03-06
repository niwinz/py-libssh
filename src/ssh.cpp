/******* Session ********/

#include <cstdio>
#include <iostream>

#include "channel.hpp"
#include "result.hpp"
#include "ssh.hpp"

namespace pyssh {

Session::Session(): Session("localhost", 22) {}
Session::Session(const std::string &hostname): Session(hostname, 22) {}

Session::Session(const std::string &hostname, const int &port) {
    this->hostname = hostname;
    this->port = port;
}

Session::~Session() {
#ifndef NDEBUG
    std::cout << "Destroing Session" << std::endl;
#endif
    if (ssh_is_connected(this->c_session)) {
        ssh_disconnect(this->c_session);
        this->c_session = NULL;
    }
}

void
Session::auth(const std::string &username, const std::string &password) {
    this->username = username;
    this->password = password;
    this->password_auth = true;

    if (this->c_session != NULL) {
        throw std::runtime_error("Cannot set auth data on session is initialized");
    }

    ssh_options_set(this->c_session, SSH_OPTIONS_USER, this->username.c_str());
}

void
Session::connect() {
    if (this->c_session != NULL) {
        this->disconnect();
    }

    this->c_session = ssh_new();

    if (this->c_session == NULL) {
        throw std::runtime_error("Cannot start session");
    }

    ssh_options_set(this->c_session, SSH_OPTIONS_HOST, this->hostname.c_str());
    ssh_options_set(this->c_session, SSH_OPTIONS_PORT, &this->port);

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

void
Session::disconnect() {
    if (ssh_is_connected(this->c_session)) {
        ssh_disconnect(this->c_session);
    } else {
        throw std::runtime_error("Session not connected");
    }
}

ssh_session
Session::get_c_session() {
    return this->c_session;
}

/* Not member functions */

boost::shared_ptr<Result>
Session::execute(const std::string &command, boost::shared_ptr<Session> session_ptr) {
    boost::shared_ptr<Channel> channel(new Channel(session_ptr));

    int rc = ssh_channel_request_exec(channel->get_c_channel(), command.c_str());
    if (rc != SSH_OK) {
        fprintf(stderr, "Error connecting to localhost: %s\n", ssh_get_error(session_ptr->get_c_session()));
        throw std::runtime_error("Cannot execute command");
    }

    return boost::shared_ptr<Result>(new Result(channel));
}

boost::shared_ptr<Session>
Session::create(const std::string &host, const int &port) {
    boost::shared_ptr<Session> session_ptr(new Session(host, port));
    return session_ptr;
}

} // End namespace
