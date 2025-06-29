#pragma once

#include <emblib/io/istream.hpp>
#include <emblib/io/ostream.hpp>
#include <arpa/inet.h>
#include <future>

namespace mpsim {

class udp_dev : public emblib::io::istream<char>, public emblib::io::ostream<char> {

public:
    explicit udp_dev(const char* send_ip, int send_port, int bind_port = -1);
    ~udp_dev();
    
    ssize_t write(const char* data, size_t size, emblib::io::timeout_t timeout) noexcept override;
    ssize_t read(char* data, size_t size, emblib::io::timeout_t timeout) noexcept override;
    bool read_async(char* data, size_t size, emblib::io::callback_t callback) noexcept override;

private:
    int m_socket;
    sockaddr_in m_send_endpoint;
    sockaddr_in m_recv_endpoint;

    std::future<void> m_read_future;
};

}