#include "udp_dev.hpp"
#include <arpa/inet.h>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

namespace mpsim {

udp_dev::udp_dev(const char* send_ip, int send_port, int bind_port)
{
    m_socket = socket(AF_INET, SOCK_DGRAM, 0);

    m_send_endpoint.sin_family = AF_INET;
    m_send_endpoint.sin_addr.s_addr = inet_addr(send_ip);
    m_send_endpoint.sin_port = htons(send_port);

    m_recv_endpoint.sin_family = AF_INET;
    m_recv_endpoint.sin_addr.s_addr = inet_addr("127.0.0.1");
    m_recv_endpoint.sin_port = htons(bind_port);

    int optval = 1;
    assert(setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) >= 0);

    if (bind_port != -1)
        assert(bind(m_socket, (sockaddr*)&m_recv_endpoint, sizeof(m_recv_endpoint)) >= 0);
}

udp_dev::~udp_dev()
{
    close(m_socket);
}

ssize_t udp_dev::write(const char* data, size_t size, emblib::milliseconds_t timeout) noexcept
{
    return sendto(m_socket, data, size, 0, (sockaddr*)&m_send_endpoint, sizeof(m_send_endpoint));
}

ssize_t udp_dev::read(char* data, size_t size, emblib::milliseconds_t timeout) noexcept
{
    return recvfrom(m_socket, data, size, 0, NULL, 0);
}

bool udp_dev::write_async(const char* data, size_t size, callback_t callback) noexcept
{
    callback(write(data, size));
    return true;
}

bool udp_dev::read_async(char* data, size_t size, callback_t callback) noexcept
{
    // if (m_read_future.valid()) {
    //     m_read_future.wait();
    // }
    m_read_future = std::async(std::launch::async, [this, &data, &size, &callback]{
        callback(read(data, size, emblib::milliseconds_t(0)));
    });
    return true;
}

}