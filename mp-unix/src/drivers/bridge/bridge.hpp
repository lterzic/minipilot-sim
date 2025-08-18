#pragma once

#include "drivers/bridge/udp_dev.hpp"
#include "pb/mpsim/bridge.pb.h"
#include <emblib/rtos/mutex.hpp>

namespace mpsim {

class bridge {

public:
    bridge(const char* server_ip, int server_port) :
        m_socket(server_ip, server_port)
    {}


    /**
     * Send a request and wait for a response with a timeout
     * If a response was not received response.success value is set to false
     * 
     * @todo Set the timeout as a parameter with a default value
     */
    pb::mpsim::Response send_request(const pb::mpsim::Request& request) noexcept;

private:
    udp_dev m_socket;
    emblib::mutex m_socket_mutex;

};

}