#include "exception.hpp"

namespace pyssh {

SshException::SshException(ssh_session csession) {
    this->code = ssh_get_error_code(csession);
    this->description = std::string(ssh_get_error(csession));
}

SshException::SshException(const SshException &e) {
    this->code = e.code;
    this->description = e.description;
}

int SshException::get_code() {
    return this->code;
}

std::string SshException::getError() {
    return this->description;
}

}
