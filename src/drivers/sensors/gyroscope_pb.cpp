#include "gyroscope_pb.hpp"

namespace mpsim {

bool gyroscope_pb::read(mp::vector<emblib::rps_t, 3>& out) noexcept
{
    // Send a protobuf message with a read gyro request and wait for a response
    pb::mpsim::Request request;
    request.mutable_read_gyro();

    pb::mpsim::Response response = m_bridge.send_request(request);
    if (!response.success() || response.response_type_case() != pb::mpsim::Response::ResponseTypeCase::kReadGyro)
        return false;

    // TODO: Remove unit constructors
    out((int)axis_e::X) = emblib::rps_t(response.read_gyro().ang_vel().x());
    out((int)axis_e::Y) = emblib::rps_t(response.read_gyro().ang_vel().y());
    out((int)axis_e::Z) = emblib::rps_t(response.read_gyro().ang_vel().z());
    return true;
}

}