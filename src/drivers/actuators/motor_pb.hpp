#pragma once

#include "drivers/bridge/bridge.hpp"
#include "emblib/driver/actuator/motor.hpp"
#include "emblib/dsp/iir.hpp"

namespace mpsim {

static constexpr auto MOTOR_FILTER_TASK_PERIOD = std::chrono::milliseconds(10); // 20 Hz
static constexpr float DT = std::chrono::duration<float>(MOTOR_FILTER_TASK_PERIOD).count();

class motor_pb : public emblib::motor {

public:
    explicit motor_pb(bridge& bridge, size_t index, bool dir_ccw, float time_constant) :
        m_bridge(bridge),
        m_index(index),
        m_dir_ccw(dir_ccw),
        m_time_constant(time_constant),
        m_speed_filter({DT/(DT + time_constant)}, {-time_constant/(DT + time_constant)})
    {}

    bool write_throttle(float throttle) noexcept override;

    bool read_throttle(float& throttle) noexcept override
    {
        throttle = m_speed_filter.get_output();
        return true;
    }

    float get_time_constant() const noexcept override
    {
        return m_time_constant;
    }

    bool get_direction() const noexcept override
    {
        return m_dir_ccw;
    }

    void simulate_actual_speed() noexcept
    {
        m_speed_filter.update(m_target_speed);
    }

private:
    bridge& m_bridge;
    size_t m_index;
    bool m_dir_ccw;
    float m_time_constant;

    float m_target_speed;
    emblib::iir_tf2<float, 1> m_speed_filter;
};

}