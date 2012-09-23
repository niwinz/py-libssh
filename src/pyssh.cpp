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

BOOST_PYTHON_MODULE(_pyssh) {
    init_bytes_module_converter();

    py::def("create_session", &pyssh::Session::create, py::return_value_policy<py::return_by_value>());
    py::def("session_execute", &pyssh::Session::execute, py::return_value_policy<py::return_by_value>());
    py::def("sftp_open_file", &pyssh::sftp_open_file, py::return_value_policy<py::return_by_value>());

    py::class_<pyssh::Session, boost::noncopyable>("Session", py::no_init)
        .def("auth", &pyssh::Session::auth)
        .def("connect", &pyssh::Session::connect)
        .def("disconnect", &pyssh::Session::disconnect);

    py::class_<pyssh::Result, boost::noncopyable>("Result", py::no_init)
        .def("next", &pyssh::Result::next)
        .add_property("return_code", &pyssh::Result::get_return_code)
        .add_property("is_finished", &pyssh::Result::is_finished);

    py::class_<pyssh::SftpFile, boost::noncopyable>("SftpFile", py::no_init)
        .def("write", &pyssh::SftpFile::write)
        .def("read",  &pyssh::SftpFile::read)
        .def("close", &pyssh::SftpFile::close)
        .def("tell", &pyssh::SftpFile::tell)
        .def("seek", &pyssh::SftpFile::seek);

    py::class_<pyssh::SftpSession>("SftpSession", py::init< boost::shared_ptr<pyssh::Session> >())
        .def("mkdir", &pyssh::SftpSession::mkdir)
        .def("put", &pyssh::SftpSession::put);

    py::register_ptr_to_python< boost::shared_ptr<pyssh::Session> >();
    py::register_ptr_to_python< boost::shared_ptr<pyssh::Result> >();
    py::register_ptr_to_python< boost::shared_ptr<pyssh::SftpSession> >();
    py::register_ptr_to_python< boost::shared_ptr<pyssh::SftpFile> >();
}
