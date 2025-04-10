# Used to compile protobuf files on every build

# Path to the build protobuf compiler
set(PROTOC ${PROJECT_BINARY_DIR}/lib/protobuf/protoc)

set(PROTO_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(PROTO_CPP_OUT ${CMAKE_CURRENT_SOURCE_DIR}/out/cpp/pb)
set(PROTO_PY_OUT ${CMAKE_CURRENT_SOURCE_DIR}/out/py)
set(PROTO_CS_OUT ${CMAKE_CURRENT_SOURCE_DIR}/out/cs)

make_directory(${PROTO_CPP_OUT})
make_directory(${PROTO_PY_OUT})
make_directory(${PROTO_CS_OUT})


# Get all protobuf src files
file(GLOB PROTO_SRCS ${PROTO_SRC_DIR}/*.proto)

# List of all generated cpp files (1 for each proto source)
set(PROTO_CC_SRCS "")
# For each source proto file, create a command which generates all the proto output files
foreach(_SRC ${PROTO_SRCS})
    # Replace the .proto extension in the filename with .pb.cc
    get_filename_component(_SRC_NAME ${_SRC} NAME_WE)
    set(_PROTO_CC ${PROTO_CPP_OUT}/${_SRC_NAME}.pb.cc)

    # Generate output files from this proto source
    add_custom_command(
        OUTPUT ${_PROTO_CC}
        DEPENDS ${_SRC}
        COMMAND ${PROTOC} --cpp_out=${PROTO_CPP_OUT} --python_out=${PROTO_PY_OUT} --csharp_out=${PROTO_CS_OUT} -I=${PROTO_SRC_DIR} ${_SRC}
    )
    # Add this cpp files to the list to be used by the static library
    list(APPEND PROTO_CC_SRCS ${_PROTO_CC})
endforeach()

# Static library with all the generated cpp files
add_library(minipilot-sim-proto STATIC ${PROTO_CC_SRCS})
target_link_libraries(minipilot-sim-proto PUBLIC libprotobuf)
target_include_directories(minipilot-sim-proto PUBLIC ${PROTO_CPP_OUT}/..)
add_dependencies(minipilot-sim-proto protoc)
