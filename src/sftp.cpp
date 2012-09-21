#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstring>

#include "sftp.hpp"
#include "sftp_file.hpp"
#include "ssh.hpp"

namespace pyssh {
namespace fs = boost::filesystem;

SftpSession::SftpSession(boost::shared_ptr<Session> s) {
    this->session = s;
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
#ifndef NDEBUG
    std::cout << "Destroing SftpSession" << std::endl;
#endif
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

boost::shared_ptr<SftpFile>
SftpSession::open(const std::string &path, const std::string &mode) {
    boost::shared_ptr<SftpSession> sftp_session_ptr;
    sftp_session_ptr.reset(this);

    return boost::shared_ptr<SftpFile>(new SftpFile(path, mode, sftp_session_ptr));
}

void
SftpSession::put(const std::string &_path, const std::string &remote_path) {
    fs::path p(_path);
    if (!fs::exists(p) or !fs::is_regular_file(p)) {
        throw std::runtime_error("Incorrect or not existent file");
    }

    fs::ifstream local_file(p, std::ios::in|std::ios::binary|std::ios::ate);
    if (!local_file.is_open()) {
        throw std::runtime_error("Cannot open file");
    }

    int buffer_size = 10;
    int size = (int) local_file.tellg();

    local_file.seekg(0, std::ios::beg);

    int access_type = O_WRONLY | O_TRUNC | O_CREAT;
    sftp_file remote_file = sftp_open(this->get_c_sftp_session(), remote_path.c_str(), access_type, S_IRWXU);
    if (remote_file == NULL) {
        throw std::runtime_error("Can't open file");
    }

    while (size > 0) {
        size = size - buffer_size;
        int get_size;

        if (size >= 0) get_size = buffer_size;
        else get_size = buffer_size + size;

        char *memblock = new char[get_size];
        local_file.read(memblock, get_size);

        int nwritten = sftp_write(remote_file, memblock, get_size);
        if (nwritten != get_size) {
            throw std::runtime_error("Can't write on file");
        }

        delete[] memblock;
    }

    local_file.close();
    sftp_close(remote_file);
}

}
