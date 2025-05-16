#include "accelerometer_pb.hpp"

namespace mpsim {

bool accelerometer_pb::read(mp::vector<emblib::mpss_t, 3>& out) noexcept
{
    // Send a protobuf message with a read acc request and wait for a response
    pb::mpsim::Request request;
    request.mutable_read_acc();

    pb::mpsim::Response response = m_bridge.send_request(request);
    if (!response.success() || response.response_type_case() != pb::mpsim::Response::ResponseTypeCase::kReadAcc)
        return false;

    // TODO: Remove unit constructors
    out((int)axis_e::X) = emblib::mpss_t(response.read_acc().acc().x());
    out((int)axis_e::Y) = emblib::mpss_t(response.read_acc().acc().y());
    out((int)axis_e::Z) = emblib::mpss_t(response.read_acc().acc().z());
    return true;
}

}