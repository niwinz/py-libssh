#include "ssh.hpp"
#include "channel.hpp"

namespace pyssh {

Channel::Channel(boost::shared_ptr<Session> session) {
    this->session = session;
    this->c_channel = ssh_channel_new(session->get_c_session());

    int rc = ssh_channel_open_session(this->c_channel);
    if (rc != SSH_OK){
        throw std::runtime_error("Cannot create channel");
    }
}

Channel::~Channel() {
#ifndef NDEBUG
    std::cout << "Destroing Channel" << std::endl;
#endif
    if (this->c_channel != NULL)
        ssh_channel_free(this->c_channel);
}

ssh_channel
Channel::get_c_channel() {
    return this->c_channel;
}

}
