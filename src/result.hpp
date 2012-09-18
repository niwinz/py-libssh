#include <libssh/libssh.h>

#ifndef _SSH_RESULT_HPP
#define _SSH_RESULT_HPP

class Bytes;

namespace pyssh {

class Channel;

/*
 * Command executrion result wrapper with treaming
 * api for transfer command output.
*/

class Result {
public:
    Result(Channel *channel);
    ~Result();

    Bytes next();
    int get_return_code();
    bool is_finished();

private:
    Channel *channel;
    bool finished = false;
    int return_code = -1;
};

}

#endif
