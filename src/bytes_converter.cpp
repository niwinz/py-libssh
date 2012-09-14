#include "bytes_converter.hpp"
#include "bytes.hpp"

PyObject* BytesToPython::convert(const Bytes &s) {
#if PY_VERSION_HEX >= 0x03000000
    return boost::python::incref(PyBytes_FromString(s.data()));
#else
    return boost::python::incref(PyString_FromString(s.data()));
#endif
}

PythonToBytes::PythonToBytes() {
    boost::python::converter::registry::push_back(&convertible, &construct, boost::python::type_id<Bytes>());
}

void* PythonToBytes::convertible(PyObject* obj_ptr) {
#if PY_VERSION_HEX >= 0x03000000
    if (!PyBytes_Check(obj_ptr)) return 0;
#else
    if (!PyString_Check(obj_ptr)) return 0;
#endif
    return obj_ptr;
}

void PythonToBytes::construct(PyObject* obj_ptr, boost::python::converter::rvalue_from_python_stage1_data* data) {
#if PY_VERSION_HEX >= 0x03000000
    const char* value = PyBytes_AsString(obj_ptr);
#else
    const char* value = PyString_AsString(obj_ptr);
#endif
    if (value == 0) boost::python::throw_error_already_set();

    void* storage = ((boost::python::converter::rvalue_from_python_storage<Bytes>*)data)->storage.bytes;
    new (storage) Bytes(value);
    data->convertible = storage;
}

void init_bytes_module_converter() {
    boost::python::to_python_converter<Bytes, BytesToPython>();
    PythonToBytes();
}
