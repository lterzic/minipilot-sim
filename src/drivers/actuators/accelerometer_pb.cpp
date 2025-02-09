#include "accelerometer_pb.hpp"
#include "pb/bridge.pb.h"
#include "pb/read_acc.pb.h"
#include <mutex>
#include <string>

static constexpr size_t RECV_BUFFER_SIZE = 256;

namespace mpsim {

bool accelerometer_pb::read_all_axes(float (&out_data)[3]) noexcept
{
    // Lock the mutex so that other drivers which communicate via this socket don't interrupt
    std::scoped_lock lock(m_socket_mutex);

    // Send a protobuf message with a read acc request
    pb::Request request;
    std::string out_bytes;
    
    request.mutable_read_acc();
    if (!request.SerializeToString(&out_bytes))
        return false;
    if (m_socket.write(out_bytes.c_str(), out_bytes.size()) != out_bytes.size())
        return false;

    // Wait for a protobuf message and assert it is of type read acc response
    char buffer[RECV_BUFFER_SIZE];
    pb::Response response;
    
    ssize_t recv_size = m_socket.read(buffer, RECV_BUFFER_SIZE);
    if (recv_size <= 0 || !response.ParseFromArray(buffer, recv_size))
        return false;
    if (response.response_type_case() != pb::Response::ResponseTypeCase::kReadAcc)
        return false;

    // Put the data in the output array in the default order (x, y, z)
    // This can be rearranged in the mp::task_accelerometer if needed
    out_data[0] = response.read_acc().acc().x();
    out_data[1] = response.read_acc().acc().y();
    out_data[2] = response.read_acc().acc().z();
    return true;
}

}