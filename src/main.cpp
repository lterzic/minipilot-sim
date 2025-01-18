#include "drivers/accelerometer_pb.hpp"
#include "drivers/gyroscope_pb.hpp"
#include "emblib/driver/io/stdio_dev.hpp"
#include "mp/main.hpp"

static const char* WINDOWS_IP = "172.21.48.1";
static const char* WSL_IP = "127.0.0.1";

int main()
{
    static emblib::stdio_dev stdio;
    
    static emblib::mutex sensor_mutex;
    static mpsim::udp_dev sensor_dev(WINDOWS_IP, 5000);
    static mpsim::udp_dev telemetry_dev(WSL_IP, 25565);

    static mpsim::accelerometer_pb accel(sensor_dev, sensor_mutex);
    static mpsim::gyroscope_pb gyro(sensor_dev, sensor_mutex);

    mp::devices_s device_drivers {
        .accelerometer = accel,
        .gyroscope = gyro,
        .log_device = &stdio,
        .telemetry_device = &telemetry_dev
    };

    return mp::main(device_drivers);
}