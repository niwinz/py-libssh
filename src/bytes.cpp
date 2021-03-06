#include "bytes.hpp"

Bytes::Bytes() {}
Bytes::Bytes(const std::string &data): std::string(data) {}
Bytes::Bytes(const Bytes &data): std::string(data) {}
Bytes::Bytes(const char *data, size_t n): std::string(data, n) {}
