#include "drivers/sensors/accelerometer_pb.hpp"
#include "drivers/sensors/gyroscope_pb.hpp"
#include "unity/state_pb.hpp"
#include "unity/vehicles/quad_x.hpp"
#include "emblib/driver/io/stdio_dev.hpp"
#include "mp/state_estimators.hpp"
#include "mp/main.hpp"

static const char* WINDOWS_IP = "172.21.48.1";
static const char* WSL_IP = "127.0.0.1";

int main()
{
    static emblib::stdio_dev stdio;
    
    static mpsim::udp_dev telemetry_dev(WSL_IP, 25565);
    static mpsim::udp_dev receiver_dev(WSL_IP, 25564, 25564);
    static mpsim::bridge bridge(WINDOWS_IP, 5000);

    static mpsim::accelerometer_pb accel(bridge);
    static mpsim::gyroscope_pb gyro(bridge);
    
    // static mpsim::unity::state_pb state_est(bridge);
    static mpsim::unity::quad_x unity_quad_x(bridge);
    static mp::ekf_inertial ekf(unity_quad_x);

    mp::devices_s device_drivers {
        .accelerometer = accel,
        .gyroscope = gyro,
        .log_device = &stdio,
        .telemetry_device = &telemetry_dev,
        .receiver_device = receiver_dev
    };

    return mp::main(device_drivers, ekf, unity_quad_x);
}