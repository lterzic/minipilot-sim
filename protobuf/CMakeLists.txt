# Used to compile protobuf files on every build

set(PROTO_SOURCE_DIR ${PROJECT_SOURCE_DIR}/protobuf/src)
set(PROTO_CPP_OUT_DIR ${PROJECT_SOURCE_DIR}/src/protobuf)
set(PROTO_CS_OUT_DIR ${PROJECT_SOURCE_DIR}/protobuf/out/cs)

make_directory(${PROTO_CPP_OUT_DIR})
make_directory(${PROTO_CS_OUT_DIR})

add_library(minipilot-sim-proto OBJECT
    ${PROTO_SOURCE_DIR}/bridge.proto
    ${PROTO_SOURCE_DIR}/common_types.proto
    ${PROTO_SOURCE_DIR}/read_acc.proto
    ${PROTO_SOURCE_DIR}/read_gyro.proto
    ${PROTO_SOURCE_DIR}/write_motor.proto
)
target_link_libraries(minipilot-sim-proto PUBLIC libprotobuf)

# Get the list of proto sources to be used for external compilation
get_target_property(PROTO_SRC_FILES minipilot-sim-proto SOURCES)

protobuf_generate(
    TARGET minipilot-sim-proto
    IMPORT_DIRS ${PROTO_SOURCE_DIR}
    PROTOC_OUT_DIR ${PROTO_CPP_OUT_DIR}
)


# TODO: Add a command for generating protobuf source files for the language specified by the argument
