# Used to compile protobuf files on every build

# Path to protoc
set(PROTOC "${PROJECT_SOURCE_DIR}/lib/minipilot-proto/lib/protoc")

# Set paths
# Generated files are placed in the pb directory so that their
# includes would have a prefix to differentiate from source files
set(PROTO_SRC_DIR "${PROJECT_SOURCE_DIR}/lib/minipilot-proto/src")
set(PROTO_GEN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/gen/pb")

# Find all proto files recursively
file(GLOB_RECURSE PROTO_FILES "${PROTO_SRC_DIR}/*.proto")

# List of generated files
set(GENERATED_FILES "")

# Process each proto file
foreach(PROTO_FILE ${PROTO_FILES})
    # Get relative path to maintain directory structure
    file(RELATIVE_PATH REL_PATH ${PROTO_SRC_DIR} ${PROTO_FILE})
    get_filename_component(REL_DIR ${REL_PATH} DIRECTORY)
    get_filename_component(FILE_NAME ${PROTO_FILE} NAME_WE)
    
    # Define the output file and directory
    set(GEN_DIR ${PROTO_GEN_DIR}/${REL_DIR})
    set(GEN_FILE "${GEN_DIR}/${FILE_NAME}.pb.cc")
    file(MAKE_DIRECTORY ${GEN_DIR})
    
    # Generate output files
    add_custom_command(
        OUTPUT ${GEN_FILE}
        DEPENDS ${PROTO_FILE}
        COMMAND ${PROTOC}
            --cpp_out=${PROTO_GEN_DIR}
            -I=${PROTO_SRC_DIR}
            ${PROTO_FILE}
    )
    
    # Add to list of generated files
    list(APPEND GENERATED_FILES ${GEN_FILE})
endforeach()

# Static library with all the generated cpp files
add_library(minipilot-sim-proto STATIC ${GENERATED_FILES})
target_link_libraries(minipilot-sim-proto PUBLIC libprotobuf)
target_include_directories(minipilot-sim-proto PUBLIC ${PROTO_GEN_DIR})
target_include_directories(minipilot-sim-proto INTERFACE ${PROTO_GEN_DIR}/..)