# Protobuf

# Flags configuration:
set(protobuf_BUILD_TESTS OFF)
set(protobuf_WITH_ZLIB OFF)
set(BUILD_SHARED_LIBS OFF)
set(_ITERATOR_DEBUG_LEVEL 0)
set(ABSL_RUN_TESTS OFF)
set(BUILD_TESTING OFF)
set(protobuf_ABSL_PROVIDER package)
set(protobuf_BUILD_LIBUPB ON)
set(protobuf_BUILD_CONFORMANCE OFF)
set(protobuf_INSTALL ON)
set(protobuf_MSVC_STATIC_RUNTIME OFF)
set(protobuf_BUILD_EXAMPLES OFF)
set(protobuf_BUILD_PROTOBUF_BINARIES ON)

if(FU_CONF_DEBUG)
    set(CMAKE_MSVC_RUNTIME_LIBRARY ${FU_MSVC_RUNTIME_DEBUG})
elseif(FU_CONF_RELEASE)
    set(CMAKE_MSVC_RUNTIME_LIBRARY ${FU_MSVC_RUNTIME_RELEASE})
endif()

# Paths:
set(PROT_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/External/protobuf)
set(absl_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../build/${FUEGO_PLATFORM}/Engine/External/abseil )

set(PROTOC ${CMAKE_CURRENT_SOURCE_DIR}/../build/${FUEGO_PLATFORM}/output/$<CONFIG>/protoc)
set(PROTO_IN ${CMAKE_CURRENT_SOURCE_DIR}/Fuego/ProtoIn)
set(PROTO_OUT ${CMAKE_CURRENT_SOURCE_DIR}/Fuego/ProtoOut)

add_subdirectory(${PROT_ROOT})


get_directory_property(PROT_TARGETS DIRECTORY ${PROT_ROOT} BUILDSYSTEM_TARGETS)
    foreach(target ${PROT_TARGETS})
        set_target_properties(${target} PROPERTIES FOLDER "3rd party/Protobuf")
    endforeach()
set_target_properties(utf8_range utf8_validity PROPERTIES FOLDER "3rd party/Protobuf")

set(SCENE_GENERATED_PROTO_SRCS "${PROTO_OUT}/SceneObjects.pb.cc" CACHE STRING "Generated Proto source file")
set(SCENE_GENERATED_PROTO_HDRS "${PROTO_OUT}/SceneObjects.pb.h" CACHE STRING "Generated Proto source file")

add_custom_command(
    OUTPUT ${SCENE_GENERATED_PROTO_SRCS} ${SCENE_GENERATED_PROTO_HDRS}
    COMMAND ${PROTOC} --proto_path=${PROTO_IN} --cpp_out=${PROTO_OUT} SceneObjects.proto
    DEPENDS ${PROTO_IN}/SceneObjects.proto  protobuf::protoc
    COMMENT "Generating Protocol Buffers sources"
    VERBATIM
)
add_custom_target(ProtoGen DEPENDS ${SCENE_GENERATED_PROTO_SRCS} ${SCENE_GENERATED_PROTO_HDRS})
