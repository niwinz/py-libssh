#ifndef _SSH_RESULT_HPP
#define _SSH_RESULT_HPP

//#include <stdexcept>
//#include <string>
//#include <libssh/libssh.h>
//#include <boost/shared_ptr.hpp>


class ExecResult {
public:
    ExecResult(Channel *channel);
    ~ExecResult();

    std::string next();
    int get_return_code();
    bool is_finished();

private:
    Channel *channel;
    bool finished = false;
    int return_code = -1;
};

#endif
