#include "drivers/actuators/accelerometer_pb.hpp"
#include "drivers/actuators/gyroscope_pb.hpp"
#include "vehicles/unity/quad_x.hpp"
#include "emblib/driver/io/stdio_dev.hpp"
#include "mp/vehicles/copter/quadcopter.hpp"
#include "mp/main.hpp"

static const char* WINDOWS_IP = "172.21.48.1";
static const char* WSL_IP = "127.0.0.1";

int main()
{
    static emblib::stdio_dev stdio;
    
    static mpsim::udp_dev telemetry_dev(WSL_IP, 25565);
    static mpsim::bridge bridge(WINDOWS_IP, 5000);

    static mpsim::accelerometer_pb accel(bridge);
    static mpsim::gyroscope_pb gyro(bridge);
    
    static mpsim::unity::quad_x unity_quad_x(bridge);

    mp::devices_s device_drivers {
        .accelerometer = accel,
        .gyroscope = gyro,
        .log_device = &stdio,
        .telemetry_device = &telemetry_dev
    };

    return mp::main(device_drivers, unity_quad_x);
}