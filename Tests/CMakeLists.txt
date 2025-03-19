# Tests
#include(${CMAKE_CURRENT_SOURCE_DIR}/../Engine/ProtoFilesMisc.cmake)
#include(${CMAKE_CURRENT_SOURCE_DIR}/../Engine/protobuf.cmake)
message(STATUS "Fuego tests are ENABLED")

# Googletest configuration
set( FUEGO_ROOT ${CMAKE_SOURCE_DIR})
set( GOOGLE_TEST_ROOT ${FUEGO_ROOT}/External/googletest)
set( GOOGLE_TEST_BUILD ${FUEGO_ROOT}/build/${FUEGO_PLATFORM}/External/googletest)

set(GTEST_SOLUTION ${FUEGO_ROOT}/build/${FUEGO_PLATFORM}/External)

set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST ON CACHE BOOL "" FORCE)
set(GTEST_HAS_ABSL OFF CACHE BOOL "" FORCE)

enable_testing()

# For Windows: Prevent overriding the parent project's compiler/linker settings
#"Use shared (DLL) run-time lib even when Google Test is built as static lib."
if(FUEGO_PLATFORM_WIN)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()
# End googletest

# Tests project configuration:
set(COMMON_TARGET_PROPERTIES PROPERTIES FOLDER "Tests")
# End of Tests configuration

# CoreLibtests configuration:
set(CORE_LIB_TESTS_ROOT ${FUEGO_ROOT}/CoreLib )
set(CORE_LIB_TESTS_BUILD ${FUEGO_ROOT}/build/${FUEGO_PLATFORM}/CoreLibTests )
# End of CoreLibtests configuration

# Main part:
set(SRC_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(TESTS_HEADERS
    ${INCLUDE_FOLDER}/Tests.h
    ${INCLUDE_FOLDER}/ProtoFilesTests.hpp
    ${SCENE_GENERATED_PROTO_HDRS}
)

set(TESTS_SOURCES
    ${SRC_FOLDER}/Tests.cpp
    ${SCENE_GENERATED_PROTO_SRCS}
)


set(CMAKE_INSTALL_PREFIX ${GOOGLE_TEST_BUILD} CACHE PATH "Install path prefix" FORCE)

add_subdirectory(${GOOGLE_TEST_ROOT} ${GOOGLE_TEST_BUILD})

set_target_properties(gtest_main gtest PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${GOOGLE_TEST_BUILD}/lib
    LIBRARY_OUTPUT_DIRECTORY ${GOOGLE_TEST_BUILD}/lib
    RUNTIME_OUTPUT_DIRECTORY ${GOOGLE_TEST_BUILD}/bin
)

add_subdirectory(${CORE_LIB_TESTS_ROOT} ${CORE_LIB_TESTS_BUILD})

set_target_properties(CoreLibTests PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CORE_LIB_TESTS_BUILD}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CORE_LIB_TESTS_BUILD}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CORE_LIB_TESTS_BUILD}/bin
)

add_executable(Tests ${TESTS_HEADERS} ${TESTS_SOURCES})

set_target_properties(gtest gtest_main ${COMMON_TARGET_PROPERTIES})
set_target_properties(Tests CoreLibTests ${COMMON_TARGET_PROPERTIES})


target_link_libraries(Tests PUBLIC
        CoreLibTests 
        GTest::gtest 
        FuegoEngine
        FuegoInterface
)

target_include_directories(Tests PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${GOOGLE_TEST_ROOT}/googletest/include
        ${FUEGO_ROOT}/CoreLib
)
