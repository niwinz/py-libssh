#ifndef _SSH_SFTP_WFILE_HPP
#define _SSH_SFTP_WFILE_HPP

#include <libssh/sftp.h>

class Bytes;

namespace pyssh {

class SftpSession;
class Session;

class SftpWFile {
public:
    SftpWFile(const std::string &filepath, SftpSession *sftp_session);
    ~SftpWFile();

    void write(const char *data);
    void write(const char *data, int length);
    void write(const Bytes &data);
    void close();

private:
    sftp_file file = NULL;
    SftpSession *sftp_session;
    Session *session;
};

}

#endif
