#include <boost/shared_ptr.hpp>
#include <boost/python.hpp>
#include <utility>

#include "ssh.hpp"

namespace py = boost::python;

Session* _connect(const std::string &hostname, const int &port) {
    Session *session =  new Session(hostname, port);
    session->connect();
    return session;
}

BOOST_PYTHON_MODULE(_pyssh) {
    py::def("connect", &_connect, py::return_value_policy<py::manage_new_object>());

    py::class_<Session>("Session", py::no_init)
        .def("disconnect", &Session::disconnect)
        .def("execute", &Session::execute, py::return_value_policy<py::manage_new_object>());

    py::class_<Result>("Result", py::no_init)
        .def("next", &Result::next)
        .add_property("return_code", &Result::get_return_code)
        .add_property("is_finished", &Result::is_finished);
}
