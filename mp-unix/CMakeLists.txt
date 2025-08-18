# Source files for minipilot-sim
add_executable(minipilot-sim
    src/drivers/actuators/motor_pb.cpp
    src/drivers/bridge/bridge.cpp
    src/drivers/bridge/udp_dev.cpp
    src/drivers/sensors/accelerometer_pb.cpp
    src/drivers/sensors/gyroscope_pb.cpp
    src/drivers/state/state_pb.cpp
    src/main.cpp
)

# Header files to be used by minipilot
target_include_directories(minipilot-sim PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

# Link dependency libraries to minipilot
target_link_libraries(minipilot-sim PRIVATE
    emblib
    minipilot
    minipilot-sim-proto
)
