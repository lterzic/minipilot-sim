cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(minipilot-sim VERSION 1.0)

# Protobuf options
set(protobuf_INSTALL OFF)
set(protobuf_BUILD_TESTS OFF)
set(protobuf_BUILD_LIBUPB OFF)

# Dependencies
add_subdirectory("lib/minipilot")
add_subdirectory("lib/protobuf")

# Generate protobuf cpp files from sources
add_subdirectory("protobuf")

# Source files for minipilot-sim
add_executable(minipilot-sim
    src/drivers/bridge/bridge.cpp
    src/drivers/bridge/udp_dev.cpp
    src/drivers/sensors/accelerometer_pb.cpp
    src/drivers/sensors/gyroscope_pb.cpp
    src/drivers/actuators/motor_pb.cpp
    src/unity/state_pb.cpp
    src/main.cpp
)

# Header files to be used by minipilot
target_include_directories(minipilot-sim PRIVATE
    "${PROJECT_SOURCE_DIR}/src"
)

# Link dependency libraries to minipilot
target_link_libraries(minipilot-sim PRIVATE
    minipilot
    minipilot-sim-proto
)
