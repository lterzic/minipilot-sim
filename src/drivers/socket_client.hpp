#pragma once

#include "emblib/driver/io/char_dev.hpp"
#include "emblib/rtos/mutex.hpp"
#include <arpa/inet.h>

namespace mpsim {

class socket_client : public emblib::char_dev {

public:
    explicit socket_client(const char* server_ip, int server_port);
    ~socket_client();
    
    ssize_t write(const char* data, size_t size) noexcept override;
    ssize_t read(char* data, size_t size) noexcept override;

private:
    int m_socket;
    sockaddr_in m_server_endpoint;

};

}