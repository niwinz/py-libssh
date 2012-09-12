/******* Session ********/

#include <cstdio>
#include <iostream>
#include "ssh.hpp"

Session::Session(const std::string &hostname, const int &port) {
    this->hostname = hostname;
    this->port = port;
}

Session::~Session() {
    std::cout << "Destroying session" << std::endl;
}

void
Session::set_auth_data(const std::string &username, const std::string &password) {
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
    ssh_disconnect(this->c_session);
}

ssh_session
Session::get_c_session() {
    return this->c_session;
}

/*
 * Standard method for executing small comamands.
*/

Result*
Session::execute(const std::string &command) {
    Channel *channel = new Channel(this);

    int rc = ssh_channel_request_exec(channel->get_c_channel(), command.c_str());
    if (rc != SSH_OK) {
        fprintf(stderr, "Error connecting to localhost: %s\n", ssh_get_error(this->c_session));
        throw std::runtime_error("Cannot execute command");
    }

    return new Result(channel);
}


/******* Channel ********/

Channel::Channel(Session *session) {
    this->session = session;
    this->c_channel = ssh_channel_new(session->get_c_session());

    int rc = ssh_channel_open_session(this->c_channel);
    if (rc != SSH_OK){
        throw std::runtime_error("Cannot create channel");
    }
}

Channel::~Channel() {
    if (this->c_channel != NULL)
        ssh_channel_free(this->c_channel);
}

ssh_channel
Channel::get_c_channel() {
    return this->c_channel;
}




/******* Result ********/

Result::Result(Channel *channel) {
    this->channel = channel;
}

Result::~Result() {
    std::cout << "Destroing ExecResult" << std::endl;
    delete this->channel;
}

std::string
Result::next() {
    if (this->finished) {
        return std::string(u8"");
    }

    char buffer[10];
    int nbytes = ssh_channel_read(channel->get_c_channel(), buffer, sizeof(buffer), 0);

    if (nbytes > 0) {
        return std::string(buffer);
    } else {
        ssh_channel_send_eof(channel->get_c_channel());

        this->finished = true;
        this->return_code = ssh_channel_get_exit_status(channel->get_c_channel());
        return std::string(u8"");
    }
}

int
Result::get_return_code() {
    return this->return_code;
}

bool
Result::is_finished() {
    return this->finished;
}
