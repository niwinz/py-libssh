#include <boost/shared_ptr.hpp>
#include <boost/python.hpp>
#include <utility>
#include <string>

#include "bytes.hpp"
#include "bytes_converter.hpp"

#include "ssh.hpp"
#include "channel.hpp"
#include "sftp.hpp"

namespace py = boost::python;

// Overloads
void (pyssh::SftpWFile::*write_text)(const char *data) = (&pyssh::SftpWFile::write);
void (pyssh::SftpWFile::*write_bytes)(const Bytes &data) = (&pyssh::SftpWFile::write);

pyssh::Session* _connect(const std::string &hostname, const int &port) {
    pyssh::Session *session =  new pyssh::Session(hostname, port);
    session->connect();
    return session;
}

BOOST_PYTHON_MODULE(_pyssh) {
    init_bytes_module_converter();

    py::def("connect", &_connect, py::return_value_policy<py::manage_new_object>());

    py::class_<pyssh::Session>("Session", py::no_init)
        .def("disconnect", &pyssh::Session::disconnect)
        .def("execute", &pyssh::Session::execute, py::return_value_policy<py::manage_new_object>());

    py::class_<pyssh::Result>("Result", py::no_init)
        .def("next", &pyssh::Result::next)
        .add_property("return_code", &pyssh::Result::get_return_code)
        .add_property("is_finished", &pyssh::Result::is_finished);

    py::class_<pyssh::SftpWFile>("SftpWFile", py::init<std::string, pyssh::SftpSession*>())
        .def("write", write_text)
        .def("write", write_bytes)
        .def("close", &pyssh::SftpWFile::close);

    py::class_<pyssh::SftpSession>("SftpSession", py::init<pyssh::Session*>())
        .def("mkdir", &pyssh::SftpSession::mkdir)
        .def("put", &pyssh::SftpSession::put)
        .def("open_for_write", &pyssh::SftpSession::open_for_write,  py::return_value_policy<py::manage_new_object>());

}
