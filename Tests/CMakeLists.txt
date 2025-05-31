# Tests include(${CMAKE_CURRENT_SOURCE_DIR}/../Engine/ProtoFilesMisc.cmake)
# include(${CMAKE_CURRENT_SOURCE_DIR}/../Engine/protobuf.cmake)
MESSAGE(STATUS "Fuego tests are ENABLED")

# Googletest configuration
SET(FUEGO_ROOT ${CMAKE_SOURCE_DIR})
SET(GOOGLE_TEST_ROOT ${FUEGO_ROOT}/External/googletest)
SET(GOOGLE_TEST_BUILD ${FUEGO_ROOT}/build/${FUEGO_PLATFORM}/External/googletest)

SET(GTEST_SOLUTION ${FUEGO_ROOT}/build/${FUEGO_PLATFORM}/External)

SET(BUILD_GMOCK
    OFF
    CACHE BOOL "" FORCE
)
SET(INSTALL_GTEST
    ON
    CACHE BOOL "" FORCE
)
SET(GTEST_HAS_ABSL
    OFF
    CACHE BOOL "" FORCE
)

ENABLE_TESTING()

# For Windows: Prevent overriding the parent project's compiler/linker settings "Use shared (DLL) run-time lib even when
# Google Test is built as static lib."
IF(FUEGO_PLATFORM_WIN)
  SET(GTEST_FORCE_SHARED_CRT
      ON
      CACHE BOOL "" FORCE
  )
ENDIF()
# End googletest

# Tests project configuration:
SET(COMMON_TARGET_PROPERTIES PROPERTIES FOLDER "Tests")
# End of Tests configuration

# CoreLibtests configuration:
SET(CORE_LIB_TESTS_ROOT ${FUEGO_ROOT}/CoreLib)
SET(CORE_LIB_TESTS_BUILD ${FUEGO_ROOT}/build/${FUEGO_PLATFORM}/CoreLibTests)
# End of CoreLibtests configuration

# Main part:
SET(SRC_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/src)
SET(INCLUDE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/include)

SET(TESTS_HEADERS ${INCLUDE_FOLDER}/Tests.h ${INCLUDE_FOLDER}/ProtoFilesTests.hpp ${INCLUDE_FOLDER}/CoreLibTests.h
                  ${SCENE_GENERATED_PROTO_HDRS}
)

SET(TESTS_SOURCES ${SRC_FOLDER}/Tests.cpp ${SCENE_GENERATED_PROTO_SRCS})

SET(CMAKE_INSTALL_PREFIX
    ${GOOGLE_TEST_BUILD}
    CACHE PATH "Install path prefix" FORCE
)

ADD_SUBDIRECTORY(${GOOGLE_TEST_ROOT} ${GOOGLE_TEST_BUILD})

SET_TARGET_PROPERTIES(
  gtest_main gtest
  PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${GOOGLE_TEST_BUILD}/lib
             LIBRARY_OUTPUT_DIRECTORY ${GOOGLE_TEST_BUILD}/lib
             RUNTIME_OUTPUT_DIRECTORY ${GOOGLE_TEST_BUILD}/bin
)

ADD_EXECUTABLE(FuegoTests ${TESTS_HEADERS} ${TESTS_SOURCES})

SET_TARGET_PROPERTIES(
  gtest gtest_main ${COMMON_TARGET_PROPERTIES}
)
SET_TARGET_PROPERTIES(
  FuegoTests ${COMMON_TARGET_PROPERTIES}
)

TARGET_LINK_LIBRARIES(FuegoTests PUBLIC CoreLib GTest::gtest FuegoEngine FuegoInterface)

TARGET_INCLUDE_DIRECTORIES(
  FuegoTests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${GOOGLE_TEST_ROOT}/googletest/include ${CORE_LIB_TESTS_ROOT}
)

TARGET_COMPILE_FEATURES(FuegoTests PUBLIC cxx_std_20)
