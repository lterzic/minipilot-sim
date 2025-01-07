#include "drivers/accelerometer_pb.hpp"
#include "drivers/gyroscope_pb.hpp"
#include "emblib/driver/io/stdio_dev.hpp"
#include "mp/main.hpp"

static const char* WINDOWS_IP = "172.21.48.1";
static const char* WSL_IP = "127.0.0.1";

int main()
{
    static emblib::stdio_dev stdio;
    static emblib::mutex socket_mutex;

    static mpsim::socket_client socket(WINDOWS_IP, 5000);
    static mpsim::accelerometer_pb accel(socket, socket_mutex);
    static mpsim::gyroscope_pb gyro(socket, socket_mutex);

    mp::devices_s device_drivers {
        .accelerometer = accel,
        .gyroscope = gyro,
        .log_device = &stdio
    };

    return mp::main(device_drivers);
}