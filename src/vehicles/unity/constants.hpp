#pragma once

#include "mp/util/math.hpp"

namespace mpsim::unity {

static inline const mp::vector3f FORWARD    = {0, 0, 1};
static inline const mp::vector3f LEFT       = {-1, 0, 0};
static inline const mp::vector3f UP         = {0, 1, 0};
static inline const mp::vector3f BACKWARD   = -FORWARD;
static inline const mp::vector3f RIGHT      = -LEFT;
static inline const mp::vector3f DOWN       = -UP;

}