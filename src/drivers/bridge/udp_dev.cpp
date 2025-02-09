#include "udp_dev.hpp"
#include <arpa/inet.h>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

namespace mpsim {

udp_dev::udp_dev(const char* server_ip, int server_port)
{
    m_socket = socket(AF_INET, SOCK_DGRAM, 0);

    m_server_endpoint.sin_family = AF_INET;
    m_server_endpoint.sin_port = htons(server_port);
    m_server_endpoint.sin_addr.s_addr = inet_addr(server_ip);
}

udp_dev::~udp_dev()
{
    close(m_socket);
}

ssize_t udp_dev::write(const char* data, size_t size) noexcept
{
    return sendto(m_socket, data, size, 0, (sockaddr*)&m_server_endpoint, sizeof(m_server_endpoint));
}

ssize_t udp_dev::read(char* data, size_t size) noexcept
{
    return recvfrom(m_socket, data, size, 0, NULL, 0);
}

}