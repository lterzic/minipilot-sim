#include "motor_pb.hpp"
#include "pb/bridge.pb.h"
#include "pb/write_motor.pb.h"
#include <mutex>
#include <string>

static constexpr size_t RECV_BUFFER_SIZE = 256;

namespace mpsim {

bool motor_pb::write_speed(float rad_per_sec) noexcept
{
    // Lock the mutex so that other drivers which communicate via this socket don't interrupt
    std::scoped_lock lock(m_socket_mutex);

    // Send a protobuf message with a write motor request
    pb::Request request;
    std::string out_bytes;
    
    request.mutable_write_motor()->set_motor_index(m_index);
    request.mutable_write_motor()->set_angular_freq(rad_per_sec);

    if (!request.SerializeToString(&out_bytes))
        return false;
    if (m_socket.write(out_bytes.c_str(), out_bytes.size()) != out_bytes.size())
        return false;

    // Wait for a protobuf message and assert it is of type write motor response
    char buffer[RECV_BUFFER_SIZE];
    pb::Response response;
    
    ssize_t recv_size = m_socket.read(buffer, RECV_BUFFER_SIZE);
    if (recv_size <= 0 || !response.ParseFromArray(buffer, recv_size))
        return false;
    if (response.response_type_case() != pb::Response::ResponseTypeCase::kWriteMotor)
        return false;

    // Current motor speed is approximated by a first order
    // linear system approaching the target speed
    m_target_speed = rad_per_sec;
    return true;
}

}