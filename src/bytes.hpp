#include <boost/python.hpp>

//class Bytes {
//public:
//    Bytes() {}
//    Bytes(const std::string &v) : value(v) {}
//    const char* data() const { return value.c_str(); }
//    const std::string& to_string() const { return value; }
//private:
//    std::string value;
//};

class Bytes: public std::string {
public:
    Bytes() {};
    Bytes(const std::string &data): std::string(data) {}
    Bytes(const Bytes &data): std::string(data) {}
};

struct Bytes_to_python_bytes {
    static PyObject* convert(const Bytes &s){
        return boost::python::incref(PyBytes_FromString(s.data()));
    }
};

struct Bytes_from_python_bytes {
    Bytes_from_python_bytes() {
        boost::python::converter::registry::push_back(&convertible, &construct, boost::python::type_id<Bytes>());
    }

    static void* convertible(PyObject* obj_ptr) {
        if (!PyBytes_Check(obj_ptr)) return 0;
        return obj_ptr;
    }

    static void construct(PyObject* obj_ptr, boost::python::converter::rvalue_from_python_stage1_data* data) {
        const char* value = PyBytes_AsString(obj_ptr);
        if (value == 0) boost::python::throw_error_already_set();

        void* storage = ((boost::python::converter::rvalue_from_python_storage<Bytes>*)data)->storage.bytes;
        new (storage) Bytes(value);
        data->convertible = storage;
    }
};

void init_bytes_module_converter() {
    boost::python::to_python_converter<Bytes, Bytes_to_python_bytes>();
    Bytes_from_python_bytes();
}
