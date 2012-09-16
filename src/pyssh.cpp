#include <boost/shared_ptr.hpp>
#include <boost/python.hpp>
#include <utility>
#include <string>

#include "ssh.hpp"
#include "channel.hpp"
#include "result.hpp"
#include "sftp.hpp"
#include "sftp_file.hpp"

#include "bytes.hpp"
#include "bytes_converter.hpp"

namespace py = boost::python;

pyssh::Session* _connect_1(const std::string &hostname, const int &port) {
    pyssh::Session *session =  new pyssh::Session(hostname, port);
    session->connect();
    return session;
}

pyssh::Session* _connect_2(const std::string &hostname) {
    pyssh::Session *session =  new pyssh::Session(hostname, 22);
    session->connect();
    return session;
}

pyssh::Session* _connect_3() {
    pyssh::Session *session =  new pyssh::Session("localhost", 22);
    session->connect();
    return session;
}


BOOST_PYTHON_MODULE(_pyssh) {
    init_bytes_module_converter();

    py::def("connect", &_connect_1, py::return_value_policy<py::manage_new_object>());
    py::def("connect", &_connect_2, py::return_value_policy<py::manage_new_object>());
    py::def("connect", &_connect_3, py::return_value_policy<py::manage_new_object>());

    py::class_<pyssh::Session>("Session", py::no_init)
        .def("disconnect", &pyssh::Session::disconnect)
        .def("execute", &pyssh::Session::execute, py::return_value_policy<py::manage_new_object>());

    py::class_<pyssh::Result>("Result", py::no_init)
        .def("next", &pyssh::Result::next)
        .add_property("return_code", &pyssh::Result::get_return_code)
        .add_property("is_finished", &pyssh::Result::is_finished);

    py::class_<pyssh::SftpFile>("SftpFile", py::no_init)
        .def("write", &pyssh::SftpFile::write)
        .def("read",  &pyssh::SftpFile::read)
        .def("close", &pyssh::SftpFile::close)
        .def("tell", &pyssh::SftpFile::tell)
        .def("seek", &pyssh::SftpFile::seek);

    py::class_<pyssh::SftpSession>("SftpSession", py::init<pyssh::Session*>())
        .def("mkdir", &pyssh::SftpSession::mkdir)
        .def("put", &pyssh::SftpSession::put)
        .def("open", &pyssh::SftpSession::open, py::return_value_policy<py::manage_new_object>());
}
