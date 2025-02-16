#pragma once

#include "drivers/bridge/bridge.hpp"
#include "emblib/driver/sensor/gyroscope.hpp"

namespace mpsim {

class gyroscope_pb : public emblib::gyroscope {

public:
    explicit gyroscope_pb(bridge& bridge) :
        m_bridge(bridge)
    {}

    bool probe() noexcept override
    {
        return true;
    }

    bool is_data_available() noexcept override
    {
        return true;
    }

    bool read_axis(axis_e axis, float& out_g) noexcept override
    {
        return false;
    }

    float get_noise_density() const noexcept override
    {
        // Noise density in mdps per sqrt(Hz) as per datasheet of LSM6DSOX
        constexpr float noise_density_mdps = 3.8;
        return noise_density_mdps * DEG_TO_RAD * 1e-3;
    }

    bool read_all_axes(float (&out_data)[3]) noexcept override;

private:
    bridge& m_bridge;

};

}