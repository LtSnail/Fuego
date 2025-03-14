# Abseil

set(ABSEIL_BASE
        base
        algorithm
        cleanup
        container
        crc
        debugging
        flags
        functional
        hash
        log
        memory
        meta
        numeric
        profiling
        random
        status
        strings
        synchronization
        time
        types
        utility
)

# Flags configuration:
set(BUILD_SHARED_LIBS OFF)
set(_ITERATOR_DEBUG_LEVEL 0)
set(ABSL_RUN_TESTS OFF)
set(ABSL_MSVC_STATIC_RUNTIME OFF)
set(ABSL_BUILD_TEST_HELPERS OFF)
set(ABSL_USE_EXTERNAL_GOOGLETEST OFF)
set(ABSL_USE_GOOGLETEST_HEAD OFF)
set(ABSL_BUILD_MONOLITHIC_SHARED_LIBS ON)
set(ABSL_ENABLE_INSTALL ON)

# Paths:
set(ABSL_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/External/abseil)


set(CMAKE_INSTALL_PREFIX ${ABSL_INST})

if(FU_CONF_DEBUG)
    set(CMAKE_MSVC_RUNTIME_LIBRARY ${FU_MSVC_RUNTIME_DEBUG})
elseif(FU_CONF_RELEASE)
    set(CMAKE_MSVC_RUNTIME_LIBRARY ${FU_MSVC_RUNTIME_RELEASE})
endif()

add_subdirectory(${ABSL_ROOT})

foreach(path ${ABSEIL_BASE})
    get_directory_property(ABSL_TARGETS DIRECTORY ${ABSL_ROOT}/absl/${path} BUILDSYSTEM_TARGETS)
    foreach(target ${ABSL_TARGETS})
        set_target_properties(${target} PROPERTIES FOLDER "3rd party/Abseil")
    endforeach()
endforeach()


