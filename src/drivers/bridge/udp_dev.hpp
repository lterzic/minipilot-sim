#pragma once

#include "emblib/driver/io/char_dev.hpp"
#include <arpa/inet.h>
#include <future>

namespace mpsim {

class udp_dev : public emblib::char_dev {

public:
    explicit udp_dev(const char* server_ip, int server_port);
    ~udp_dev();
    
    ssize_t write(const char* data, size_t size) noexcept override;
    ssize_t read(char* data, size_t size) noexcept override;
    bool write_async(const char* data, size_t size, callback_t callback) noexcept override;
    bool read_async(char* data, size_t size, callback_t callback) noexcept override;
    
    bool probe() noexcept override
    {
        return m_socket != -1;
    }

    bool is_async_available() noexcept override
    {
        return true;
    }

private:
    int m_socket;
    sockaddr_in m_server_endpoint;

    std::future<void> m_read_future;
};

}