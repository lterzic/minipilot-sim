#include "mp/main.hpp"
#include "drivers/accelerometer.hpp"
#include "emblib/driver/io/stdio_dev.hpp"

static const char* WINDOWS_IP = "172.21.48.1";
static const char* WSL_IP = "127.0.0.1";

int main()
{
    static emblib::stdio_dev stdio;
    static mpsim::socket_client socket(WINDOWS_IP, 5000);
    static emblib::mutex socket_mutex;
    static mpsim::accelerometer_pb accel(socket, socket_mutex);

    mp::devices_s device_drivers {
        .accelerometer = accel,
        .log_device = &stdio
    };

    return mp::main(device_drivers);
}