#include "bridge.hpp"
#include <iostream>
#include <string>

namespace mpsim {

// Should be large enough to hold the largest response pb message
inline constexpr size_t RECV_BUFFER_SIZE = 512;
// How long to wait for a response before returing a fail
inline constexpr auto RESPONSE_WAIT_TIMEOUT = emblib::io::timeout_t(10);

static inline void log_bridge_error(const pb::mpsim::Request& request, std::string msg)
{
    std::cout << "BRIDGE: " << msg << "\n";
    std::cout << "Request:\n" << request.DebugString() << "\n";
}

pb::mpsim::Response bridge::send_request(const pb::mpsim::Request& request) noexcept
{
    // Lock the mutex so that other drivers which communicate via this socket don't interrupt
    emblib::scoped_lock lock(m_socket_mutex);

    // Response success value is false by default if we return before actually receiving the response
    pb::mpsim::Response response;
    response.set_success(false);

    // Serialize and send a protobuf message with the given request
    std::string out_bytes;
    if (!request.SerializeToString(&out_bytes)) {
        log_bridge_error(request, "Serialization failed");
        return response;
    }
    if (m_socket.write(out_bytes.c_str(), out_bytes.size(), emblib::io::timeout_t(-1)) != out_bytes.size()) {
        log_bridge_error(request, "Socket write failed");
        return response;
    }

    // Wait for a protobuf message and try to parse it
    char buffer[RECV_BUFFER_SIZE];
    ssize_t recv_size = 0;
    while (recv_size <= 0) {
        // Wait for the first non-empty packet
        recv_size = m_socket.read(buffer, RECV_BUFFER_SIZE, RESPONSE_WAIT_TIMEOUT);
    }
    
    if (!response.ParseFromArray(buffer, recv_size)) {
        response.set_success(false);
        log_bridge_error(request, "Parsing failed");
    }

    return response;
}

}