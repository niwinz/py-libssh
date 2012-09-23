#ifndef _SSH_SESSION_HPP
#define _SSH_SESSION_HPP

#include <boost/shared_ptr.hpp>
#include <libssh/libssh.h>
#include <stdexcept>
#include <string>

namespace pyssh {

class Channel;
class Result;

class Session {
public:
    Session();
    Session(const std::string &hostname="localhost");
    Session(const std::string &hostname, const int &port);
    ~Session();

    static boost::shared_ptr<Session> create(const std::string &host, const int &port);
    static boost::shared_ptr<Result> execute(const std::string &command, boost::shared_ptr<Session> session_ptr);

    void auth(const std::string &username, const std::string &password);
    void connect();
    void disconnect();

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
