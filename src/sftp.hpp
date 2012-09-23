#ifndef _SSH_SFTP_HPP
#define _SSH_SFTP_HPP

#include <libssh/sftp.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/shared_ptr.hpp>

class Bytes;

namespace pyssh {

class Session;
class SftpFile;

class SftpSession {
public:
    //SftpSession(Session *session);
    SftpSession(boost::shared_ptr<Session> s);
    ~SftpSession();

    void mkdir(const std::string &);
    void put(const std::string &, const std::string &);

    sftp_session get_c_sftp_session();

private:
    sftp_session c_sftp_session = NULL;
    boost::shared_ptr<Session> session;
};

boost::shared_ptr<SftpFile> sftp_open_file(const std::string &path, const std::string &mode, boost::shared_ptr<SftpSession> sftp_session_ptr);

}
#endif
