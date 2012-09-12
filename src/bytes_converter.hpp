#ifndef _BYTES_CONVERTER_HPP
#define _BYTES_CONVERTER_HPP

#include <boost/python.hpp>
#include <string>

class Bytes;

//struct Bytes_to_python_bytes {
//    static PyObject* convert(const Bytes &s){
//        return boost::python::incref(PyBytes_FromString(s.data()));
//    }
//};

class BytesToPython {
public:
    static PyObject* convert(const Bytes &s);
};

//class PythonToBytes {
//    PythonToBytes(); {
//        boost::python::converter::registry::push_back(&convertible, &construct, boost::python::type_id<Bytes>());
//    }
//
//    static void* convertible(PyObject* obj_ptr) {
//        if (!PyBytes_Check(obj_ptr)) return 0;
//        return obj_ptr;
//    }
//
//    static void construct(PyObject* obj_ptr, boost::python::converter::rvalue_from_python_stage1_data* data) {
//        const char* value = PyBytes_AsString(obj_ptr);
//        if (value == 0) boost::python::throw_error_already_set();
//
//        void* storage = ((boost::python::converter::rvalue_from_python_storage<Bytes>*)data)->storage.bytes;
//        new (storage) Bytes(value);
//        data->convertible = storage;
//    }
//};

class PythonToBytes {
public:
    PythonToBytes();
    static void* convertible(PyObject* obj_ptr);
    static void construct(PyObject* obj_ptr, boost::python::converter::rvalue_from_python_stage1_data* data);
};

void init_bytes_module_converter();

#endif
