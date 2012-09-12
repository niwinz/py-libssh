#include <libssh/libssh.h>
#include <stdexcept>
#include <string>

#ifndef _SSH_SESSION_HPP
#define _SSH_SESSION_HPP

class Result;

class Session {
public:
    Session(const std::string &hostname, const int &port);
    ~Session();

    void set_auth_data(const std::string &username, const std::string &password);
    void connect();
    void disconnect();

    Result* execute(const std::string &command);
    ssh_session get_c_session();

private:
    ssh_session c_session = NULL;
    bool password_auth = false;

    std::string hostname;
    int port;

    std::string username;
    std::string password;
};

#endif


#ifndef _SSH_CHANNEL_HPP
#define _SSH_CHANNEL_HPP

/*
 * Simple wrapper for ssh_channel for
 * correct memory liberation a la c++.
*/

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


#ifndef _SSH_RESULT_HPP
#define _SSH_RESULT_HPP

/*
 * Command executrion result wrapper with treaming
 * api for transfer command output.
*/

class Result {
public:
    Result(Channel *channel);
    ~Result();

    std::string next();
    int get_return_code();
    bool is_finished();

private:
    Channel *channel;
    bool finished = false;
    int return_code = -1;
};

#endif
