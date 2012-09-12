#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstring>

#include "sftp.hpp"
#include "ssh.hpp"

namespace pyssh {
namespace fs = boost::filesystem;

SftpSession::SftpSession(Session *session) {
    this->session = session;
    this->c_sftp_session = sftp_new(session->get_c_session());

    if (this->c_sftp_session == NULL) {
        //fprintf(stderr, "Error allocating SFTP session: %s\n", ssh_get_error(session->get_c_session()));
        throw std::runtime_error("Cannot create sftp session");
    }

    int rc = sftp_init(this->c_sftp_session);
    if (rc != SSH_OK) {
        //fprintf(stderr, "Error initializing SFTP session: %s.\n", sftp_get_error(this->c_sftp_session));
        throw std::runtime_error("Cannot create sftp session");
    }
}

SftpSession::~SftpSession() {
    std::cout << "Destroing SftpSession" << std::endl;

    sftp_free(this->c_sftp_session);
    this->c_sftp_session = NULL;
}

void
SftpSession::mkdir(const std::string &dirname) {
    int rc = sftp_mkdir(this->c_sftp_session, dirname.c_str(), S_IRWXU);
    if (rc != SSH_OK){
        //fprintf(stderr, "Can't create directory: %s\n", sftp_get_error(this->c_sftp_session));
        throw std::runtime_error("Can't create directory");
    }
}

sftp_session
SftpSession::get_c_sftp_session() {
    return this->c_sftp_session;
}

SftpWFile*
SftpSession::open_for_write(const std::string &path) {
    return new SftpWFile(path, this);
}

void
SftpSession::put(const std::string &_path, const std::string &remote_path) {
    fs::path p(_path);
    if (!fs::exists(p) or !fs::is_regular_file(p)) {
        throw std::runtime_error("Incorrect or not existent file");
    }

    fs::ifstream file(p, std::ios::in|std::ios::binary|std::ios::ate);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file");
    }

    int buffer_size = 10;
    int size = (int) file.tellg();

    file.seekg (0, std::ios::beg);

    SftpWFile *sftp_file = new SftpWFile(remote_path, this);

    while (size > 0) {
        size = size - buffer_size;
        int get_size;

        if (size >= 0) get_size = buffer_size;
        else get_size = buffer_size + size;

        char *memblock = new char[get_size];
        file.read(memblock, get_size);

        sftp_file->write(memblock, get_size);
        delete[] memblock;
    }

    file.close();
    sftp_file->close();
    delete sftp_file;
}

/******** Files *********/

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
