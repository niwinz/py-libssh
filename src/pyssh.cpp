#include <boost/python.hpp>
#include <string>
#include <utility>

#include "common.hpp"
#include "session.hpp"
#include "channel.hpp"
#include "result.hpp"

namespace py = boost::python;

sessionptr new_session(const std::string &hostname, const int &port=22) {
    sessionptr ptr = sessionptr(new Session(hostname, port));
    ptr->connect();
    return ptr;
}

BOOST_PYTHON_MODULE(_pyssh) {
    py::def("connect", &new_session);

    py::class_<Session>("Session", py::no_init)
        .def("disconnect", &Session::disconnect)
        .def("execute", &Session::execute);

    py::class_<ExecResult>("Result", py::no_init)
        .def("next", &ExecResult::next)
        .add_property("return_code", &ExecResult::get_return_code)
        .add_property("is_finished", &ExecResult::is_finished);

    py::register_ptr_to_python<sessionptr>();
    py::register_ptr_to_python<execresultptr>();
}
