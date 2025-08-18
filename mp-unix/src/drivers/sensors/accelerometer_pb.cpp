#include "accelerometer_pb.hpp"

namespace mpsim {

bool accelerometer_pb::read(emblib::accelerometer::data_t& out) noexcept
{
    // Send a protobuf message with a read acc request and wait for a response
    pb::mpsim::Request request;
    request.mutable_read_acc();

    pb::mpsim::Response response = m_bridge.send_request(request);
    if (!response.success() || response.response_type_case() != pb::mpsim::Response::ResponseTypeCase::kReadAcc)
        return false;

    // TODO: Remove unit constructors
    out[0] = emblib::mpss_t(response.read_acc().acc().x());
    out[1] = emblib::mpss_t(response.read_acc().acc().y());
    out[2] = emblib::mpss_t(response.read_acc().acc().z());
    return true;
}

}