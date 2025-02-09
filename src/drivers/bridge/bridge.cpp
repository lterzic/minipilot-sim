#include "bridge.hpp"
#include <string>

namespace mpsim {

static constexpr size_t RECV_BUFFER_SIZE = 512;

pb::Response bridge::send_request(const pb::Request& request) noexcept
{
    // Lock the mutex so that other drivers which communicate via this socket don't interrupt
    emblib::scoped_lock lock(m_socket_mutex);

    // Response success value is false by default if we return before actually receiving the response
    pb::Response response;
    response.set_success(false);

    // Serialize and send a protobuf message with the given request
    std::string out_bytes;
    if (!request.SerializeToString(&out_bytes))
        return response;
    if (m_socket.write(out_bytes.c_str(), out_bytes.size()) != out_bytes.size())
        return response;

    // Wait for a protobuf message and try to parse it
    char buffer[RECV_BUFFER_SIZE];
    // TODO: Add a read timeout
    ssize_t recv_size = m_socket.read(buffer, RECV_BUFFER_SIZE);
    if (recv_size <= 0 || !response.ParseFromArray(buffer, recv_size))
        response.set_success(false);

    return response;
}

}