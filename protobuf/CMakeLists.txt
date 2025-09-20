# Used to compile protobuf files during CMake configuration

# Path to protoc
set(PROTOC "${PROJECT_SOURCE_DIR}/lib/minipilot-proto/lib/protoc")

# Set paths
set(PROTO_SRC_DIR "${PROJECT_SOURCE_DIR}/lib/minipilot-proto/src")
set(PROTO_GEN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/gen")

# Find all proto files recursively
file(GLOB_RECURSE SRC_FILES "${PROTO_SRC_DIR}/*.proto")

# Generate files
file(REMOVE_RECURSE ${PROTO_GEN_DIR})
file(MAKE_DIRECTORY ${PROTO_GEN_DIR}/cpp/pb)
file(MAKE_DIRECTORY ${PROTO_GEN_DIR}/csharp)
execute_process(COMMAND
    ${PROTOC}
    --cpp_out=${PROTO_GEN_DIR}/cpp/pb
    --csharp_out=${PROTO_GEN_DIR}/csharp
    -I=${PROTO_SRC_DIR}
    ${SRC_FILES}
)

# Static library with all the generated cpp files
file(GLOB_RECURSE GEN_FILES "${PROTO_GEN_DIR}/*.pb.cc")
add_library(minipilot-sim-proto STATIC ${GEN_FILES})
target_link_libraries(minipilot-sim-proto PUBLIC libprotobuf)
target_include_directories(minipilot-sim-proto PUBLIC ${PROTO_GEN_DIR}/cpp/pb)
target_include_directories(minipilot-sim-proto INTERFACE ${PROTO_GEN_DIR}/cpp)