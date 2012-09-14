#include <string>
#include <stdexcept>
#include <iostream>

#include "channel.hpp"
#include "result.hpp"

namespace pyssh {


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
        return std::string("");
    }

    char buffer[1024];
    int nbytes = ssh_channel_read(channel->get_c_channel(), buffer, sizeof(buffer), 0);

    if (nbytes > 0) {
        return std::string(buffer);
    } else {
        ssh_channel_send_eof(channel->get_c_channel());

        this->finished = true;
        this->return_code = ssh_channel_get_exit_status(channel->get_c_channel());
        return std::string("");
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
