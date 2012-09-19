#ifndef _SSH_RESULT_HPP
#define _SSH_RESULT_HPP

#include <libssh/libssh.h>
#include <boost/shared_ptr.hpp>

class Bytes;

namespace pyssh {

class Channel;

/*
 * Command executrion result wrapper with treaming
 * api for transfer command output.
*/

class Result {
public:
    Result(boost::shared_ptr<Channel> channel);
    ~Result();

    Bytes next();
    int get_return_code();
    bool is_finished();

private:
    boost::shared_ptr<Channel> channel;
    bool finished = false;
    int return_code = -1;
};

}

#endif
