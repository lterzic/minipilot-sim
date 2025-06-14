#pragma once

#include <emblib/driver/io/io_dev.hpp>
#include <iostream>

namespace mpsim {

class stdio_dev : public emblib::io_dev {

public:
    ssize_t write(const char* data, size_t size, emblib::milliseconds_t timeout) noexcept override
    {
        std::cout.write(data, size);
        return size;
    }

    ssize_t read(char* data, size_t size, emblib::milliseconds_t timeout) noexcept override
    {
        return -1;
    }

    bool probe(timeout_t timeout) noexcept override
    {
        return true;
    }

};

}