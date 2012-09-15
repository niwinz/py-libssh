#include "bytes.hpp"
#include "sftp_wfile.hpp"
#include "sftp.hpp"

namespace pyssh {

SftpWFile::SftpWFile(const std::string &path, SftpSession *sftp_session) {
    this->sftp_session = sftp_session;

    int access_type = O_WRONLY | O_CREAT | O_TRUNC;
    this->file = sftp_open(this->sftp_session->get_c_sftp_session(), path.c_str(), access_type, S_IRWXU);

    if (this->file == NULL) {
        throw std::runtime_error("Can't open file");
    }
}

SftpWFile::~SftpWFile() {
    std::cout << "Destroing SftpWFile" << std::endl;

    if (this->file != NULL) {
        sftp_close(this->file);
        this->file = NULL;
    }

    this->sftp_session = NULL;
}

void
SftpWFile::write(const char *data) {
    int length = std::strlen(data);
    this->write(data, length);
}

void
SftpWFile::write(const char *data, int length) {
    int nwritten = sftp_write(this->file, data, length);

    if (nwritten != length) {
        throw std::runtime_error("Can't write on file");
    }
}

void
SftpWFile::write(const Bytes &data) {
    this->write(data.c_str(), data.size());
}

void
SftpWFile::close() {
    if (this->file != NULL) {
        sftp_close(this->file);
        this->file = NULL;
    }
}

}
