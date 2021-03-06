#ifndef _SSH_SFTP_FILE_HPP
#define _SSH_SFTP_FILE_HPP

#include <string>
#include <libssh/sftp.h>
#include <boost/shared_ptr.hpp>


class Bytes;

namespace pyssh {

class SftpSession;
class Session;


class SftpFile {
public:
    SftpFile(const std::string &filepath, const std::string &mode,
        boost::shared_ptr<SftpSession> sftp_session_ptr);
    ~SftpFile();

    void write(const Bytes &data);

    Bytes read(int bytes=-1);

    bool seek(uint64_t new_offset);
    uint64_t tell();
    void close();

private:
    size_t parse_mode(const std::string &);
    void _write(const char *data, const int &length);

    friend SftpSession;

    sftp_file file = NULL;
    boost::shared_ptr<SftpSession> sftp_session;
};

}

#endif
