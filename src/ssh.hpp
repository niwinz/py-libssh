#include <libssh/libssh.h>
#include <stdexcept>
#include <string>

#ifndef _SSH_SESSION_HPP
#define _SSH_SESSION_HPP


namespace pyssh {

class Channel;
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

} // End namespace

#endif
