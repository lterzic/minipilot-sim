#include "state_pb.hpp"
#include "unity/constants.hpp"

namespace mpsim {

state_pb::state_pb(bridge& bridge) :
    state_estimator_periodic(PERIOD, m_stack),
    m_bridge(bridge)
{}

inline mp::vector3f pb_to_mp_vector3f(const pb::mp::Vector3f& v) noexcept
{
    return unity::MP_TRANSFORM.matmul(mp::vector3f({v.x(), v.y(), v.z()}));
}

void state_pb::iteration(float dt) noexcept
{
    // Send a protobuf message with a read state request and wait for a response
    pb::mpsim::Request request;
    request.mutable_read_state();

    pb::mpsim::Response response = m_bridge.send_request(request);
    if (!response.success() || !response.has_read_state())
        return;

    m_state = response.read_state().state();
}

mp::state_s state_pb::create_state() const noexcept
{
    const auto& p = m_state.position();
    const auto& v = m_state.velocity();
    const auto& a = m_state.acceleration();
    const auto& w = m_state.angular_velocity();
    const auto& q = m_state.rotation();
    
    return {
        .position = pb_to_mp_vector3f(p),
        .velocity = pb_to_mp_vector3f(v),
        .acceleration = pb_to_mp_vector3f(a),
        .angular_velocity = -pb_to_mp_vector3f(w),
        .rotationq = {q.w(), -unity::MP_TRANSFORM.matmul(mp::vector3f({q.x(), q.y(), q.z()}))}
    };
}

}