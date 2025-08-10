#pragma once

#include "drivers/bridge/bridge.hpp"
#include <mp/state.hpp>

namespace mpsim {



class state_pb : public mp::state_estimator_periodic {

    static constexpr size_t STACK_SIZE = 2048;
    static constexpr mp::milliseconds_t PERIOD = mp::milliseconds_t(20);

public:
    explicit state_pb(bridge& bridge);

private:
    /**
     * Request the exact state from the simulator
     */
    void iteration(float dt) noexcept override;

    /**
     * Convert protobuf state representation to mp state
     */
    mp::state_s create_state() const noexcept override;

private:
    bridge& m_bridge;
    pb::mp::State m_state;
    
    emblib::task_stack_t<STACK_SIZE> m_stack;
};

}