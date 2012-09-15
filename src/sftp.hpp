#ifndef _SSH_SFTP_HPP
#define _SSH_SFTP_HPP

#include <libssh/sftp.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

class Bytes;

namespace pyssh {

class Session;
class SftpWFile;

class SftpSession {
public:
    SftpSession(Session *session);
    ~SftpSession();

    void mkdir(const std::string &);
    void put(const std::string &, const std::string &);

    SftpWFile* open_for_write(const std::string &);

    sftp_session get_c_sftp_session();

private:
    sftp_session c_sftp_session = NULL;
    Session *session;
};

#endif

}
