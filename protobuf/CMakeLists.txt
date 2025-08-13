# Used to compile protobuf files on every build

# Path to protoc
set(PROTOC "${PROJECT_SOURCE_DIR}/lib/minipilot-proto/lib/protoc")

# Set paths
set(PROTO_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(PROTO_GEN_CPP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/gen/pb")
set(PROTO_GEN_CS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/gen/cs")

# Find all proto files recursively
file(GLOB_RECURSE PROTO_FILES FOLLOW_SYMLINKS "${PROTO_SRC_DIR}/*.proto")

# List of generated files
set(GENERATED_FILES "")

# Process each proto file
foreach(PROTO_FILE ${PROTO_FILES})
    # Get relative path to maintain directory structure
    file(RELATIVE_PATH REL_PATH ${PROTO_SRC_DIR} ${PROTO_FILE})
    get_filename_component(REL_DIR ${REL_PATH} DIRECTORY)
    get_filename_component(FILE_NAME ${PROTO_FILE} NAME_WE)
    
    # Generated files are placed in the pb directory so that their
    # includes would have a prefix to differentiate from source files
    set(GEN_DIR_CPP ${PROTO_GEN_CPP_DIR}/${REL_DIR})
    file(MAKE_DIRECTORY ${GEN_DIR_CPP})
    # C# files for Unity are placed in the cs folder
    set(GEN_DIR_CS ${PROTO_GEN_CS_DIR}/${REL_DIR})
    file(MAKE_DIRECTORY ${GEN_DIR_CS})
    
    # Relative file name
    set(GEN_FILE "${GEN_DIR_CPP}/${FILE_NAME}.pb.cc")
    
    # Generate output files
    add_custom_command(
        OUTPUT ${GEN_FILE}
        DEPENDS ${PROTO_FILE}
        COMMAND ${PROTOC}
            --cpp_out=${PROTO_GEN_CPP_DIR}
            --csharp_out=${PROTO_GEN_CS_DIR}
            -I=${PROTO_SRC_DIR}
            ${PROTO_FILE}
    )
    
    # Add to list of generated files
    list(APPEND GENERATED_FILES ${GEN_FILE})
endforeach()

# Static library with all the generated cpp files
add_library(minipilot-sim-proto STATIC ${GENERATED_FILES})
target_link_libraries(minipilot-sim-proto PUBLIC libprotobuf)
target_include_directories(minipilot-sim-proto PUBLIC ${PROTO_GEN_CPP_DIR})
target_include_directories(minipilot-sim-proto INTERFACE ${PROTO_GEN_CPP_DIR}/..)