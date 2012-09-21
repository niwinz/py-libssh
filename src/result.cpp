#include <string>
#include <cstring>
#include <stdexcept>
#include <iostream>

#include "channel.hpp"
#include "result.hpp"
#include "bytes.hpp"

namespace pyssh {


Result::Result(boost::shared_ptr<Channel> channel) {
    this->channel = channel;
}

Result::~Result() {
#ifndef NDEBUG
    std::cout << "Destroing Result" << std::endl;
#endif
}

Bytes
Result::next() {
    if (this->finished) return Bytes("");

    int status = ssh_channel_is_closed(channel->get_c_channel());
    if (status != 0) {
        throw std::runtime_error("Channel is closed");
    }

    char buffer[10];
    int nbytes = ssh_channel_read(channel->get_c_channel(), buffer, sizeof(buffer), 0);

    if (nbytes > 0) {
        return Bytes(buffer, nbytes);
    } else {
        ssh_channel_send_eof(channel->get_c_channel());

        this->finished = true;
        this->return_code = ssh_channel_get_exit_status(channel->get_c_channel());
        return Bytes("");
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

}
