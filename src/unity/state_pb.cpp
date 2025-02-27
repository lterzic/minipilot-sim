#include "state_pb.hpp"
#include "constants.hpp"

namespace mpsim::unity {

inline mp::vector3f pb_to_mp_vector3f(const pb::Vector3f& v) noexcept
{
    return MP_TRANSFORM.matmul(mp::vector3f({v.x(), v.y(), v.z()}));
}

void state_pb::update(const mp::sensor_data_s& input, float dt) noexcept
{
    // Send a message requesting the exact state
    pb::Request request;
    request.mutable_get_state();

    pb::Response response = m_bridge.send_request(request);
    if (!response.success() || !response.has_get_state())
        return;

    const auto& p = response.get_state().position();
    const auto& v = response.get_state().velocity();
    const auto& a = response.get_state().acceleration();
    const auto& w = response.get_state().angular_velocity();
    const auto& q = response.get_state().rotationq();
    
    m_state.position = pb_to_mp_vector3f(p);
    m_state.velocity = pb_to_mp_vector3f(v);
    m_state.acceleration = pb_to_mp_vector3f(a);
    m_state.angular_velocity = -pb_to_mp_vector3f(w);
    m_state.rotationq = {q.w(), -MP_TRANSFORM.matmul(mp::vector3f({q.x(), q.y(), q.z()}))};
}

}