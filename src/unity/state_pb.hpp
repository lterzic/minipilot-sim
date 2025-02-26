#pragma once

#include "drivers/bridge/bridge.hpp"
#include "mp/state_estimators.hpp"

namespace mpsim::unity {

class state_pb : public mp::state_estimator {

public:
    explicit state_pb(bridge& bridge) :
        m_bridge(bridge)
    {}

    void update(const mp::sensor_data_s& input, float dt) noexcept override;

    mp::state_s get_state() const noexcept override
    {
        return m_state;
    }

private:
    bridge& m_bridge;
    mp::state_s m_state;

};

}