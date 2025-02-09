#pragma once

#include "drivers/bridge/bridge.hpp"
#include "emblib/driver/actuator/motor.hpp"
#include "emblib/dsp/iir.hpp"

namespace mpsim {

static constexpr float UNITY_MOTOR_TIME_CONST = 0.0001f;
static constexpr auto MOTOR_FILTER_TASK_PERIOD = std::chrono::milliseconds(10); // 20 Hz
static constexpr float DT = std::chrono::duration<float>(MOTOR_FILTER_TASK_PERIOD).count();

class motor_pb : public emblib::motor {

public:
    explicit motor_pb(bridge& bridge, size_t index) :
        m_bridge(bridge),
        m_index(index),
        m_speed_filter({DT/(DT + UNITY_MOTOR_TIME_CONST)}, {-UNITY_MOTOR_TIME_CONST/(DT + UNITY_MOTOR_TIME_CONST)})
    {}

    bool write_speed(float rad_per_sec) noexcept override;

    bool read_speed(float& rad_per_sec) noexcept override
    {
        rad_per_sec = m_speed_filter.get_output();
        return true;
    }

    float get_time_constant() const noexcept override
    {
        return UNITY_MOTOR_TIME_CONST;
    }

    void simulate_actual_speed() noexcept
    {
        m_speed_filter.update(m_target_speed);
    }

private:
    bridge& m_bridge;
    size_t m_index;

    float m_target_speed;
    emblib::iir_tf2<float, 1> m_speed_filter;
};

}