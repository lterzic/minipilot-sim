#pragma once

#include "drivers/bridge/bridge.hpp"
#include "drivers/actuators/motor_pb.hpp"
#include "unity/constants.hpp"
#include "mp/vehicles.hpp"

namespace mpsim::unity {

class quad_x : public mp::quadcopter {
    
    // Match with the order in the unity prefab
    enum class motor_index_e {
        FR, FL, BL, BR
    };

    static inline const mp::quadcopter_params_s PARAMETERS {
        {
            .mass = 1.2f,
            .moment_of_inertia = emblib::vector3f({1, 1, 2}).as_diagonal() * 0.1f,
            .lin_drag_c = 1.f
        },
        .width_half = 0.2f,
        .length_half = 0.2f,
        .thrust_coeff = PROP_MOTOR_THRUST_COEFF,
        .torque_coeff = PROP_MOTOR_TORQUE_COEFF
    };

    static inline const sensor_config_s SENSOR_CONFIG {
        .accelerometer_transform = MP_TRANSFORM,
        .gyroscope_transform = -MP_TRANSFORM
    };

public:
    explicit quad_x(bridge& bridge) :
        quadcopter(
            PARAMETERS,
            m_controller,
            {.fl = m_motor_fl, .fr = m_motor_fr, .bl = m_motor_bl, .br = m_motor_br}
        ),
        m_motor_fr(bridge, (int)motor_index_e::FR, true, PROP_MOTOR_TIME_CONSTANT),
        m_motor_fl(bridge, (int)motor_index_e::FL, false, PROP_MOTOR_TIME_CONSTANT),
        m_motor_bl(bridge, (int)motor_index_e::BL, true, PROP_MOTOR_TIME_CONSTANT),
        m_motor_br(bridge, (int)motor_index_e::BR, false, PROP_MOTOR_TIME_CONSTANT),
        m_controller(PARAMETERS)
    {}

    bool init() noexcept override
    {
        return true;
    }

    const sensor_config_s& get_sensor_config() const noexcept override
    {
        return SENSOR_CONFIG;
    }

private:
    motor_pb m_motor_fr;
    motor_pb m_motor_fl;
    motor_pb m_motor_bl;
    motor_pb m_motor_br;

    mp::copter_controller_pid m_controller;
};

}