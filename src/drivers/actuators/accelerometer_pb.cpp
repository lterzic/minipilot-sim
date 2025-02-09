#include "accelerometer_pb.hpp"

namespace mpsim {

bool accelerometer_pb::read_all_axes(float (&out_data)[3]) noexcept
{
    // Send a protobuf message with a read acc request and wait for a response
    pb::Request request;
    request.mutable_read_acc();

    pb::Response response = m_bridge.send_request(request);
    if (!response.success() || response.response_type_case() != pb::Response::ResponseTypeCase::kReadAcc)
        return false;

    // Put the data in the output array in the default order (x, y, z)
    // This can be rearranged in the mp::task_accelerometer if needed
    out_data[(int)axis_e::X] = response.read_acc().acc().x();
    out_data[(int)axis_e::Y] = response.read_acc().acc().y();
    out_data[(int)axis_e::Z] = response.read_acc().acc().z();
    return true;
}

}