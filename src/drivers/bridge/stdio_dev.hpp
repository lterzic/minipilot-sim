#pragma once

#include <emblib/io/ostream.hpp>
#include <iostream>

namespace mpsim {

class stdio_dev : public emblib::io::ostream<char> {

public:
    ssize_t write(const char* data, size_t size, emblib::io::timeout_t timeout) noexcept override
    {
        std::cout.write(data, size);
        return size;
    }
};

}