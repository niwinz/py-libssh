#include "common.hpp"
#include "result.hpp"
#include "channel.hpp"

ExecResult::ExecResult(Channel *channel) {
    this->channel = channel;
}

ExecResult::~ExecResult() {
    std::cout << "Destroing ExecResult" << std::endl;
}

int ExecResult::get_return_code() {
    return this->return_code;
}

bool ExecResult::is_finished() {
    return this->finished;
}

std::string ExecResult::next() {
    if (this->finished) {
        return std::string("");
    }

    char buffer[10];
    int nbytes = ssh_channel_read(channel->get_c_channel(), buffer, sizeof(buffer), 0);

    if (nbytes > 0) {
        return std::string(buffer);
    } else {
        ssh_channel_send_eof(channel->get_c_channel());

        this->finished = true;
        this->return_code = ssh_channel_get_exit_status(channel->get_c_channel());

        delete this->channel;
        return std::string("");
    }
}
