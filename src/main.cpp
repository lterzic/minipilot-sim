#include "drivers/bridge/bridge.hpp"
#include "drivers/bridge/stdio_dev.hpp"
#include "drivers/sensors/accelerometer_pb.hpp"
#include "drivers/sensors/gyroscope_pb.hpp"
#include "drivers/state/state_pb.hpp"
#include "unity/constants.hpp"
#include "unity/vehicles/quad_x.hpp"
#include <mp/comm.hpp>
#include <mp/logging.hpp>
#include <mp/sensors.hpp>
#include <mp/state.hpp>

static const char* WINDOWS_IP       = "172.21.48.1";
static const char* LOCALHOST_IP     = "127.0.0.1";

static const uint16_t LINK_TX_PORT  = 25565;
static const uint16_t LINK_RX_PORT  = 25564;
static const uint16_t BRIDGE_PORT   = 5000;

emblib::units::milliseconds<size_t> mp::get_time_since_start() noexcept
{
    static const auto start = std::chrono::steady_clock::now();
    const auto now = std::chrono::steady_clock::now();
    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
    return emblib::units::milliseconds<size_t>(elapsed);
}

static mp::sensor_manager& create_sensors(mpsim::bridge& bridge)
{
    static mpsim::accelerometer_pb accel(bridge);
    static mp::accelerometer_reader accelerometer_reader(mp::accelerometer_config_s {
        .device = accel,
        .mutex = nullptr,
        .transform = mpsim::unity::MP_TRANSFORM
    });

    static mpsim::gyroscope_pb gyro(bridge);
    static mp::gyroscope_reader gyroscope_reader(mp::gyroscope_config_s {
        .device = gyro,
        .mutex = nullptr,
        .transform = -mpsim::unity::MP_TRANSFORM
    });

    static mp::sensor_manager sensor_manager({
        .accelerometer = &accelerometer_reader,
        .gyroscope = &gyroscope_reader
    });

    return sensor_manager;
}

int main()
{
    // Create RX and TX modules for link with the user
    static mpsim::udp_dev link_dev(LOCALHOST_IP, LINK_TX_PORT, LINK_RX_PORT);
    static mp::pb_rx rx(link_dev);
    static mp::pb_tx tx(link_dev);

    // Default logger output is the protobuf link
    static mp::log_sink_pb sink_pb(tx);
    mp::logger::get_instance().add_sink(sink_pb);

    // Standard output is available in the simulator
    static mpsim::stdio_dev stdio;
    static mp::log_sink_text sink_stdout(stdio);
    mp::logger::get_instance().add_sink(sink_stdout);
    
    // Create the simulated vehicle and sensors
    static mpsim::bridge bridge(LOCALHOST_IP, BRIDGE_PORT);
    static mp::sensor_manager& sensor_manager = create_sensors(bridge);
    static mpsim::unity::quad_x unity_quad_x(bridge);

    // Using EKF as the state estimator by default
    static mp::model_simple simple_quad_model(0.01, 0.01);
    static mp::ekf ekf(sensor_manager, simple_quad_model);
    
    // Exact state estimator for use while testing control
    static mpsim::state_pb state_pb(bridge);

    // Switch to state_pb for exact state
    static mp::state_estimator& state_estimator = ekf;

    // Using the PID controller by default
    static mp::copter_control_pid controller(unity_quad_x, state_estimator, mpsim::unity::quad_x::PARAMETERS);
    static mp::rx_handler_command rx_command(&controller);
    rx.set_handler(pb_mp_UplinkMessage_command_tag, &rx_command);

    // Setup transmitter modules
    static mp::telemetry telemetry(tx, sensor_manager, state_estimator);

    // Give execution control to the RTOS
    log_info("Starting the scheduler...");
    emblib::task::start_scheduler();

    // Should never reach this
    return 1;

}