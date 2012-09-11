#include "common.hpp"
#include "channel.hpp"
#include "session.hpp"

Channel::Channel(Session *session) {
    this->session = session;
    this->c_channel = ssh_channel_new(session->get_c_session());

    int rc = ssh_channel_open_session(this->c_channel);
    if (rc != SSH_OK){
        throw std::runtime_error("Cannot create channel");
    }
}

Channel::~Channel() {
    if (this->c_channel != NULL) {
        std::cout << "Destroying channel" << std::endl;
        ssh_channel_free(this->c_channel);
    }
}

ssh_channel Channel::get_c_channel() {
    return this->c_channel;
}
