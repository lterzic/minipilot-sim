#pragma once

#include "drivers/bridge/bridge.hpp"
#include <mp/math.hpp>
#include <emblib/driver/sensor/accelerometer.hpp>

namespace mpsim {

class accelerometer_pb : public emblib::accelerometer {

public:
    explicit accelerometer_pb(bridge& bridge) :
        m_bridge(bridge)
    {}

    const char* get_name() const noexcept override
    {
        return "PB Accel";
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
        // Noise density in micro gs per sqrt(Hz) as per datasheet of LSM6DSOX
        constexpr emblib::units::standard_gravity<float> noise_density(75 * 1e-6);
        // Change to the same units as the data type
        return emblib::mpss_t(noise_density).value();
    }

    bool read_axis(size_t axis, emblib::mpss_t& out) noexcept override
    {
        // Reading a single axis not used
        return false;
    }

    bool read(emblib::accelerometer::data_t& out) noexcept override;

private:
    bridge& m_bridge;

};

}