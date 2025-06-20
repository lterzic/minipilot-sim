#pragma once

#include <emblib/driver/io/io_dev.hpp>
#include <arpa/inet.h>
#include <future>

namespace mpsim {

class udp_dev : public emblib::io_dev {

public:
    explicit udp_dev(const char* send_ip, int send_port, int bind_port = -1);
    ~udp_dev();
    
    ssize_t write(const char* data, size_t size, emblib::milliseconds_t timeout = emblib::milliseconds_t(0)) noexcept override;
    ssize_t read(char* data, size_t size, emblib::milliseconds_t timeout) noexcept override;
    bool write_async(const char* data, size_t size, callback_t callback) noexcept override;
    bool read_async(char* data, size_t size, callback_t callback) noexcept override;
    
    bool probe(emblib::milliseconds_t timeout) noexcept override
    {
        return m_socket != -1;
    }

    bool is_async_available() noexcept override
    {
        return true;
    }

private:
    int m_socket;
    sockaddr_in m_send_endpoint;
    sockaddr_in m_recv_endpoint;

    std::future<void> m_read_future;
};

}