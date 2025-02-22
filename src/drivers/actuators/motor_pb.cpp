#include "motor_pb.hpp"

namespace mpsim {

bool motor_pb::write_throttle(float throttle) noexcept
{
    // Send a protobuf message with a write motor request
    pb::Request request;
    request.mutable_write_motor()->set_motor_index(m_index);
    request.mutable_write_motor()->set_throttle(throttle);

    // Wait for a protobuf message and assert it is of type write motor response
    pb::Response response = m_bridge.send_request(request);    
    if (!response.success() || response.response_type_case() != pb::Response::ResponseTypeCase::kWriteMotor)
        return false;

    // Current motor speed is approximated by a first order
    // linear system approaching the target speed
    m_target_speed = throttle;
    return true;
}

}