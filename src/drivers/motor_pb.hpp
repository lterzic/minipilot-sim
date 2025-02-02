#pragma once

#include "udp_dev.hpp"
#include "emblib/driver/actuator/motor.hpp"
#include "emblib/dsp/iir.hpp"
#include "emblib/rtos/mutex.hpp"

namespace mpsim {

static constexpr float UNITY_MOTOR_TIME_CONST = 0.0001f;
static constexpr auto MOTOR_FILTER_TASK_PERIOD = std::chrono::milliseconds(10); // 20 Hz
static constexpr float dt = std::chrono::duration<float>(MOTOR_FILTER_TASK_PERIOD).count();

class motor_pb : public emblib::motor {

public:
    explicit motor_pb(size_t index, udp_dev& socket, emblib::mutex& socket_mutex) :
        m_index(index),
        m_socket(socket),
        m_socket_mutex(socket_mutex),
        m_speed_filter({dt/(dt + UNITY_MOTOR_TIME_CONST)}, {-UNITY_MOTOR_TIME_CONST/(dt + UNITY_MOTOR_TIME_CONST)})
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
    size_t m_index;

    udp_dev& m_socket;
    emblib::mutex& m_socket_mutex;

    float m_target_speed;
    emblib::iir_tf2<float, 1> m_speed_filter;
};

}