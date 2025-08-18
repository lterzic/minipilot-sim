#pragma once

#include <mp/utils.hpp>

namespace mpsim::unity {

static inline const mp::vector3f FORWARD    = {0, 0, 1};
static inline const mp::vector3f LEFT       = {-1, 0, 0};
static inline const mp::vector3f UP         = {0, 1, 0};
static inline const mp::vector3f BACKWARD   = -FORWARD;
static inline const mp::vector3f RIGHT      = -LEFT;
static inline const mp::vector3f DOWN       = -UP;

// Transforms the unity coordinate space to minipilot coordinate space
static inline const mp::matrix3f MP_TRANSFORM =
    mp::FORWARD.matmul(FORWARD.transpose()) +
    mp::LEFT.matmul(LEFT.transpose()) +
    mp::UP.matmul(UP.transpose());

static inline constexpr float PROP_MOTOR_THRUST_COEFF = 10;
static inline constexpr float PROP_MOTOR_TORQUE_COEFF = 0.1f;
static inline constexpr float PROP_MOTOR_TIME_CONSTANT = 0.1f;

}