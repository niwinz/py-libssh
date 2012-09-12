#include "bytes_converter.hpp"
#include "bytes.hpp"

PyObject* BytesToPython::convert(const Bytes &s) {
    return boost::python::incref(PyBytes_FromString(s.data()));
}

PythonToBytes::PythonToBytes() {
    boost::python::converter::registry::push_back(&convertible, &construct, boost::python::type_id<Bytes>());
}

void* PythonToBytes::convertible(PyObject* obj_ptr) {
    if (!PyBytes_Check(obj_ptr)) return 0;
    return obj_ptr;
}

void PythonToBytes::construct(PyObject* obj_ptr, boost::python::converter::rvalue_from_python_stage1_data* data) {
    const char* value = PyBytes_AsString(obj_ptr);
    if (value == 0) boost::python::throw_error_already_set();

    void* storage = ((boost::python::converter::rvalue_from_python_storage<Bytes>*)data)->storage.bytes;
    new (storage) Bytes(value);
    data->convertible = storage;
}

void init_bytes_module_converter() {
    boost::python::to_python_converter<Bytes, BytesToPython>();
    PythonToBytes();
}
