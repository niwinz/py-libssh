#ifndef _SSH_CHANNEL_HPP
#define _SSH_CHANNEL_HPP

#include <libssh/libssh.h>
#include <boost/shared_ptr.hpp>
#include <stdexcept>
#include <string>

/*
 * Simple wrapper for ssh_channel for
 * correct memory liberation a la c++.
*/

namespace pyssh {

class Session;

class Channel {
public:
    Channel(boost::shared_ptr<Session> session);
    ~Channel();

    ssh_channel get_c_channel();

private:
    boost::shared_ptr<Session> session;
    ssh_channel c_channel = NULL;
};

}

#endif
