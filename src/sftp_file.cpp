#include "sftp_file.hpp"
#include "sftp.hpp"
#include "bytes.hpp"

namespace pyssh {

SftpFile::SftpFile(const std::string &filepath, const std::string &mode, SftpSession *sftp_session) {
    this->sftp_session = sftp_session;

    size_t access_type = this->parse_mode(mode);
    this->file = sftp_open(this->sftp_session->get_c_sftp_session(), filepath.c_str(), access_type, S_IRWXU);

    if (this->file == NULL) {
        throw std::runtime_error("Can't open file");
    }
}

SftpFile::~SftpFile() {
    if (this->file != NULL) {
        sftp_close(this->file);
        this->file = NULL;
    }
    this->sftp_session = NULL;
}

void
SftpFile::write(const Bytes &data) {
    int nwritten = sftp_write(this->file, data.c_str(), data.size());
    if (nwritten != data.size()) {
        throw std::runtime_error("Can't write on file");
    }
}

void
SftpFile::close() {
    if (this->file != NULL) {
        sftp_close(this->file);
        this->file = NULL;
    }
}

bool
SftpFile::seek(uint64_t new_offset) {
    int ret = sftp_seek64(this->file, new_offset);
    if (ret == 0) return true;
    else return false;
}

Bytes
SftpFile::read(size_t bytes) {
    char *memblock = new char[bytes];
    size_t readed = sftp_read(this->file, memblock,  bytes);

    if (readed == 0) return Bytes("");
    if (readed != bytes) throw std::runtime_error("error on read");
    return Bytes(memblock);
}

Bytes
SftpFile::read() {
    char *memblock = new char[1024];
    size_t readed = sftp_read(this->file, memblock,  1024);

    if (readed == 0) return Bytes("");
    if (readed != 1024) throw std::runtime_error("error on read");

    Bytes data(memblock);

    while(readed > 0) {
        readed = sftp_read(this->file, memblock,  1024);
        if (readed == 0) break;
        if (readed != 1024) throw std::runtime_error("error on read");

        data.append(memblock);
    }

    return data;
}

size_t
SftpFile::parse_mode(const std::string &mode) {
    size_t _mode = 0;

    if (mode.find("r") != std::string::npos and mode.find("w") != std::string::npos) {
        _mode = _mode | O_RDWR;
    } else if (mode.find("r") != std::string::npos) {
        if (mode.find("+") != std::string::npos) {
            _mode = _mode | O_RDWR;
        } else {
            _mode = _mode | O_RDONLY;
        }
    } else if (mode.find("w") != std::string::npos) {
        if (mode.find("+") != std::string::npos) {
            _mode = _mode | O_RDWR;
        } else {
            _mode = _mode | O_WRONLY;
        }
        _mode = _mode | O_TRUNC;
    } else {
        throw std::runtime_error("Incorrect mode");
    }

    _mode = _mode | O_CREAT;
    return _mode;
}

}
