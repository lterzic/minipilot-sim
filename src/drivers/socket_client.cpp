#include "socket_client.hpp"
#include <arpa/inet.h>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

namespace mpsim {

socket_client::socket_client(const char* server_ip, int server_port)
{
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);

    if (connect(m_socket, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Can't establish socket conenction!\n";
    } else {
        std::cout << "Socket connection successful!\n";
    }
}

socket_client::~socket_client()
{
    close(m_socket);
}

ssize_t socket_client::write(const char* data, size_t size) noexcept
{
    return send(m_socket, data, size, 0);
}

ssize_t socket_client::read(char* data, size_t size) noexcept
{
    return recv(m_socket, data, size, 0);
}

}