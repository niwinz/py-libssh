#ifndef _BYTES_HPP
#define _BYTES_HPP

#include <boost/python.hpp>
#include <string>

class Bytes: public std::string {
public:
    Bytes();
    Bytes(const std::string &data);
    Bytes(const Bytes &data);
};

# endif
