#ifndef _SSH_RESULT_HPP
#define _SSH_RESULT_HPP

/*
 * Command executrion result wrapper with treaming
 * api for transfer command output.
*/

class ExecResult {
public:
    ExecResult(Channel *channel) {
        this->channel = channel;
    }

    ~ExecResult() {}

    std::string next() {
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

    int get_return_code() { return this->return_code; }
    bool is_finished() { return this->finished; }

private:
    Channel *channel;
    bool finished = false;
    int return_code = -1;
};

#endif
