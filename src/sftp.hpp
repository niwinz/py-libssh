#ifndef _SSH_SFTP_HPP
#define _SSH_SFTP_HPP

#include <libssh/sftp.h>
#include <sys/stat.h>
#include <fcntl.h>

namespace pyssh {

class Session;
class SftpWFile;

class SftpSession {
public:
    SftpSession(Session *session);
    ~SftpSession();

    void mkdir(const std::string &path);
    //void remove(const std::string &path);
    SftpWFile* open_for_write(const std::string &path);

    sftp_session get_c_sftp_session();

private:
    sftp_session c_sftp_session = NULL;
    Session *session;
};


#endif

#ifndef _SSH_SFTP_FILE_HPP
#define _SSH_SFTP_FILE_HPP

class SftpWFile {
public:
    SftpWFile(const std::string &filepath, SftpSession *sftp_session);
    ~SftpWFile();

    void write(const char *data);
    void close();

private:
    sftp_file file = NULL;
    SftpSession *sftp_session;
    Session *session;
};

#endif

}
