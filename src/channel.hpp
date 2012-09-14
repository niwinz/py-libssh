#include <libssh/libssh.h>
#include <stdexcept>
#include <string>

#ifndef _SSH_CHANNEL_HPP
#define _SSH_CHANNEL_HPP

/*
 * Simple wrapper for ssh_channel for
 * correct memory liberation a la c++.
*/

namespace pyssh {

class Session;

class Channel {
public:
    Channel(Session *session);
    ~Channel();

    ssh_channel get_c_channel();

private:
    Session *session;
    ssh_channel c_channel = NULL;
};

}

#endif
