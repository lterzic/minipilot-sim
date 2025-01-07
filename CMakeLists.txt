cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(minipilot-sim VERSION 1.0)

# Dependencies
add_subdirectory("lib/minipilot")

# Generate protobuf cpp files from sources
add_subdirectory("protobuf")

# Source files for minipilot-sim
add_executable(minipilot-sim
    src/drivers/socket_client.cpp
    src/drivers/accelerometer_pb.cpp
    src/drivers/gyroscope_pb.cpp
    src/protobuf/bridge.pb.cc
    src/protobuf/common_types.pb.cc
    src/protobuf/read_acc.pb.cc
    src/protobuf/read_gyro.pb.cc
    src/protobuf/write_motor.pb.cc
    src/main.cpp
)

# Header files to be used by minipilot
target_include_directories(minipilot-sim PRIVATE
    "${PROJECT_SOURCE_DIR}/src"
)

# Link dependency libraries to minipilot
target_link_libraries(minipilot-sim PRIVATE
    minipilot
)

# Wait for protobuf compilation before starting to compile minipilot
add_dependencies(minipilot-sim minipilot-sim-proto)
