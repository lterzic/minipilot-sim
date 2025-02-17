#pragma once

#include "drivers/bridge/bridge.hpp"
#include "drivers/actuators/motor_pb.hpp"
#include "vehicles/unity/constants.hpp"
#include "mp/util/constants.hpp"
#include "mp/vehicles/copter/quadcopter.hpp"

namespace mpsim::unity {

class quad_x : public mp::quadcopter {
    
    // Match with the order in the unity prefab
    enum class motor_index_e {
        FR, FL, BL, BR
    };

    static inline const mp::quadcopter_params_s PARAMETERS {
        {
            .mass = 1.f,
            .moment_of_inertia = emblib::vector3f({1, 1, 2}).as_diagonal(),
            .lin_drag_c = 0.3f
        },
        .arm_length = 0.5f,
        .arm_angle = M_PI_2f,
        .thrust_coeff = 0.1f,
        .torque_coeff = 0.01f
    };

    static inline const mp::matrix3f UNITY_TO_MP_FRAME =
        mp::UP.matmul(UP.transpose()) +
        mp::LEFT.matmul(LEFT.transpose()) +
        mp::FORWARD.matmul(FORWARD.transpose());
    

    static inline const sensor_config_s SENSOR_CONFIG {
        .accelerometer_transform = UNITY_TO_MP_FRAME,
        .gyroscope_transform = UNITY_TO_MP_FRAME
    };

public:
    explicit quad_x(bridge& bridge) :
        quadcopter(
            PARAMETERS,
            {.fl = m_motor_fl, .fr = m_motor_fr, .bl = m_motor_bl, .br = m_motor_br}
        ),
        m_motor_fr(bridge, (int)motor_index_e::FR),
        m_motor_fl(bridge, (int)motor_index_e::FL),
        m_motor_bl(bridge, (int)motor_index_e::BL),
        m_motor_br(bridge, (int)motor_index_e::BR)
    {}

    const sensor_config_s& get_sensor_config() const noexcept override
    {
        return SENSOR_CONFIG;
    }

private:
    motor_pb m_motor_fr;
    motor_pb m_motor_fl;
    motor_pb m_motor_bl;
    motor_pb m_motor_br;

};

}