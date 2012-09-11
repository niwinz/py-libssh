#ifndef _SSH_SESSION_HPP
#define _SSH_SESSION_HPP

#include <libssh/libssh.h>
#include "common.hpp"

class Session {
public:
    Session(const std::string &hostname = "localhost", const int &port = 22);
    ~Session();

    void start_session();
    void set_auth_data(const std::string &username, const std::string &password);
    void connect();
    void disconnect();

    //int execute(const std::string &command);
    execresultptr execute(const std::string &command);

    ssh_session get_c_session();

private:
    ssh_session c_session = NULL;

    std::string hostname;
    int port;

    std::string username;
    std::string password;

    bool password_auth = false;
};

#endif
