#include "drivers/actuators/accelerometer_pb.hpp"
#include "drivers/actuators/gyroscope_pb.hpp"
#include "drivers/actuators/motor_pb.hpp"
#include "emblib/driver/io/stdio_dev.hpp"
#include "mp/main.hpp"
#include "mp/dynamics/copter/quadcopter.hpp"

static const char* WINDOWS_IP = "172.21.48.1";
static const char* WSL_IP = "127.0.0.1";

int main()
{
    static emblib::stdio_dev stdio;
    
    static emblib::mutex bridge_dev_mutex;
    static mpsim::udp_dev bridge_dev(WINDOWS_IP, 5000);
    static mpsim::udp_dev telemetry_dev(WSL_IP, 25565);

    static mpsim::accelerometer_pb accel(bridge_dev, bridge_dev_mutex);
    static mpsim::gyroscope_pb gyro(bridge_dev, bridge_dev_mutex);
    
    // Motor indices are manually matched with unity quadcopter bridge script setup
    static mpsim::motor_pb unity_motor_fr(0, bridge_dev, bridge_dev_mutex);
    static mpsim::motor_pb unity_motor_fl(1, bridge_dev, bridge_dev_mutex);
    static mpsim::motor_pb unity_motor_bl(2, bridge_dev, bridge_dev_mutex);
    static mpsim::motor_pb unity_motor_br(3, bridge_dev, bridge_dev_mutex);

    static mp::quadcopter_actuators_s unity_motors {
        .fl = unity_motor_fl, .fr = unity_motor_fr, .bl = unity_motor_bl, .br = unity_motor_br
    };

    static mp::quadcopter_params_s unity_quad_params {
        {
            .mass = 1.f,
            .inertia_matrix = emblib::vector3f({1, 1, 1}).as_diagonal(),
            .lin_drag_c = emblib::vector3f({0.1f, 0.1f, 0.1f})
        },
        .arm_length = 0.5f,
        .arm_angle = M_PI_2f,
        .thrust_coeff = 0.1f,
        .torque_coeff = 0.01f
    };

    static mp::quadcopter unity_quadcopter(unity_quad_params, unity_motors);

    mp::devices_s device_drivers {
        .accelerometer = accel,
        .gyroscope = gyro,
        .log_device = &stdio,
        .telemetry_device = &telemetry_dev
    };

    return mp::main(device_drivers, unity_quadcopter);
}