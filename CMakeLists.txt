cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(minipilot-sim VERSION 1.0)

# Dependencies
add_subdirectory("lib/minipilot")

# Build protobuf lib from minipilot-proto since
# it's not built by default
set(protobuf_INSTALL                    OFF)
set(protobuf_BUILD_TESTS                OFF)
set(protobuf_BUILD_CONFORMANCE          OFF)
set(protobuf_BUILD_EXAMPLES             OFF)
set(protobuf_BUILD_PROTOBUF_BINARIES    ON)
set(protobuf_BUILD_PROTOC_BINARIES      OFF)
set(protobuf_BUILD_LIBPROTOC            OFF)
set(protobuf_BUILD_LIBUPB               OFF)
set(protobuf_DISABLE_RTTI               OFF)
set(protobuf_TEST_XML_OUTDIR            OFF)
set(protobuf_ALLOW_CCACHE               OFF)
# Add the protobuf library
add_subdirectory("${PROJECT_SOURCE_DIR}/lib/minipilot-proto/lib/protobuf")
# Generate protobuf cpp files from sources
add_subdirectory("protobuf")

# Source files for minipilot-sim
add_executable(minipilot-sim
    src/drivers/bridge/bridge.cpp
    src/drivers/bridge/udp_dev.cpp
    src/drivers/sensors/accelerometer_pb.cpp
    src/drivers/sensors/gyroscope_pb.cpp
    src/drivers/actuators/motor_pb.cpp
    src/main.cpp
)

# Header files to be used by minipilot
target_include_directories(minipilot-sim PRIVATE
    "${PROJECT_SOURCE_DIR}/src"
)

# Link dependency libraries to minipilot
target_link_libraries(minipilot-sim PRIVATE
    emblib
    minipilot
    minipilot-sim-proto
)
