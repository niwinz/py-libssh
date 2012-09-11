#ifndef _SSH_CHANNEL_HPP
#define _SSH_CHANNEL_HPP

#include <libssh/libssh.h>

class Channel {
public:
    Channel(Session *session);
    ~Channel();

    ssh_channel get_c_channel();
private:
    Session *session;
    ssh_channel c_channel = NULL;
};


#endif
