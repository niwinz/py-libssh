#ifndef _SSH_EXCEPTION_HPP
#define _SSH_EXCEPTION_HPP

#include <libssh/libssh.h>
#include <string>

namespace pyssh {

class SshException {
public:
    SshException(ssh_session csession);
    SshException(const SshException &e);
    int getCode();
    std::string getError();
private:
    int code;
    std::string description;
};

}
