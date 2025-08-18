#pragma once

#include "drivers/bridge/bridge.hpp"
#include <emblib/driver/sensor/gyroscope.hpp>

namespace mpsim {

class gyroscope_pb : public emblib::gyroscope {

public:
    explicit gyroscope_pb(bridge& bridge) :
        m_bridge(bridge)
    {}

    const char* get_name() const noexcept override
    {
        return "PB Gyro";
    }

    bool probe() noexcept override
    {
        return true;
    }

    rate_t set_rate(rate_t rate) noexcept override
    {
        // Can't be changed
        return get_rate();
    }

    rate_t get_rate() const noexcept override
    {
        // Unity fixed update rate is 250Hz
        return rate_t(250);
    }

    bool is_ready() noexcept override
    {
        return true;
    }

    float get_noise_density() const noexcept override
    {
        // Noise density in milli dps per sqrt(Hz) as per datasheet of LSM6DSOX
        constexpr emblib::units::degrees_per_second<float> noise_density(3.8 * 1e-3);
        // Change to the same units as the data type
        return emblib::rps_t(noise_density).value();
    }

    bool read_axis(size_t axis, emblib::rps_t& out) noexcept override
    {
        // Reading a single axis not used
        return false;
    }

    bool read(emblib::gyroscope::data_t& out) noexcept override;

private:
    bridge& m_bridge;

};

}