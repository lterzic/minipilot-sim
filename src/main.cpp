#include "drivers/bridge/bridge.hpp"
#include "drivers/bridge/stdio_dev.hpp"
#include "drivers/sensors/accelerometer_pb.hpp"
#include "drivers/sensors/gyroscope_pb.hpp"
#include "unity/vehicles/quad_x.hpp"
#include <mp/state_estimators.hpp>
#include <mp/log_handlers.hpp>
#include <mp/mp.hpp>
#include <chrono>

static const char* WINDOWS_IP = "172.21.48.1";
static const char* LOCALHOST_IP = "127.0.0.1";

namespace mp {

static const auto start_time = std::chrono::steady_clock::now();

emblib::units::milliseconds<size_t> get_time_since_start() noexcept
{
    const auto now = std::chrono::steady_clock::now();
    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time);
    return emblib::units::milliseconds<size_t>(elapsed);
}

}

int main()
{
    static mpsim::stdio_dev stdio;
    static mp::log_device stdio_log_dev(stdio, mp::log_level_e::DEBUG);
    
    static etl::list log_string_devices {&stdio_log_dev};
    static mp::log_handler_string string_handler(log_string_devices);

    static auto log_handlers = etl::make_list<mp::log_handler*>(&string_handler);
    
    static mpsim::udp_dev telemetry_dev(LOCALHOST_IP, 25565);
    static mpsim::udp_dev receiver_dev(LOCALHOST_IP, 25564, 25564);
    static mpsim::bridge bridge(LOCALHOST_IP, 5000);

    static mpsim::accelerometer_pb accel(bridge);
    static mpsim::gyroscope_pb gyro(bridge);
    
    static mpsim::unity::quad_x unity_quad_x(bridge);
    static mp::ekf_inertial ekf_inertial(unity_quad_x);

    mp::devices_s device_drivers {
        .accelerometer = {.sensor = accel, .transform = mpsim::unity::MP_TRANSFORM},
        .gyroscope = {.sensor = gyro, .transform = -mpsim::unity::MP_TRANSFORM},
        .telemetry_device = &telemetry_dev,
        .receiver_device = receiver_dev,
        .log_handlers = log_handlers
    };

    return mp::main(device_drivers, ekf_inertial, unity_quad_x);
}