#include <boost/shared_ptr.hpp>
#include <boost/python.hpp>
#include <utility>

#include "ssh.hpp"
#include "sftp.hpp"

namespace py = boost::python;

pyssh::Session* _connect(const std::string &hostname, const int &port) {
    pyssh::Session *session =  new pyssh::Session(hostname, port);
    session->connect();
    return session;
}

BOOST_PYTHON_MODULE(_pyssh) {
    py::def("connect", &_connect, py::return_value_policy<py::manage_new_object>());

    py::class_<pyssh::Session>("Session", py::no_init)
        .def("disconnect", &pyssh::Session::disconnect)
        .def("execute", &pyssh::Session::execute, py::return_value_policy<py::manage_new_object>());

    py::class_<pyssh::Result>("Result", py::no_init)
        .def("next", &pyssh::Result::next)
        .add_property("return_code", &pyssh::Result::get_return_code)
        .add_property("is_finished", &pyssh::Result::is_finished);

    py::class_<pyssh::SftpWFile>("SftpWFile", py::init<std::string, pyssh::SftpSession*>())
        .def("write", &pyssh::SftpWFile::write)
        .def("close", &pyssh::SftpWFile::close);

    py::class_<pyssh::SftpSession>("SftpSession", py::init<pyssh::Session*>())
        .def("mkdir", &pyssh::SftpSession::mkdir)
        .def("open_for_write", &pyssh::SftpSession::open_for_write,  py::return_value_policy<py::manage_new_object>());
}
