#ifndef _SSH_SFTP_FILE_HPP
#define _SSH_SFTP_FILE_HPP

#include <string>
#include <libssh/sftp.h>


class Bytes;

namespace pyssh {

class SftpSession;
class Session;


class SftpFile {
public:
    SftpFile(const std::string &filepath, const std::string &mode, SftpSession *sftp_session);
    ~SftpFile();

    void write(const Bytes &data);

    Bytes read(size_t bytes);
    Bytes read();

    bool seek(uint64_t new_offset);
    void close();

private:
    size_t parse_mode(const std::string &);

    sftp_file file = NULL;
    SftpSession *sftp_session;
    Session *session;
};

}

#endif
