#pragma once

#include "drivers/bridge/udp_dev.hpp"
#include "emblib/driver/sensor/accelerometer.hpp"
#include "emblib/rtos/mutex.hpp"

namespace mpsim {

class accelerometer_pb : public emblib::accelerometer {

public:
    explicit accelerometer_pb(udp_dev& socket, emblib::mutex& socket_mutex) :
        m_socket(socket),
        m_socket_mutex(socket_mutex)
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
        return 0;
    }

    bool read_all_axes(float (&out_data)[3]) noexcept override;

private:
    udp_dev& m_socket;
    emblib::mutex& m_socket_mutex;

};

}