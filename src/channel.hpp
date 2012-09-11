#ifndef _SSH_CHANNEL_HPP
#define _SSH_CHANNEL_HPP

#include <libssh/libssh.h>

/*
 * Simple wrapper for ssh_channel for
 * correct memory liberation a la c++.
*/

class Channel {
public:
    Channel(Session *session)
    {
        this->session = session;
        this->c_channel = ssh_channel_new(session->get_c_session());

        int rc = ssh_channel_open_session(this->c_channel);
        if (rc != SSH_OK){
            throw std::runtime_error("Cannot create channel");
        }
    }

    ~Channel()
    {
        if (this->c_channel != NULL)
            ssh_channel_free(this->c_channel);
    }

    ssh_channel get_c_channel()
    {
        return this->c_channel;
    }

private:
    Session *session;
    ssh_channel c_channel = NULL;
};

#endif
