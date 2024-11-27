# CoreLib
set(CMAKE_CXX_STANDARD 20)

set(CORELIB_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/CoreLibConcepts.h
    ${CMAKE_CURRENT_SOURCE_DIR}/UniquePtr.h
    ${CMAKE_CURRENT_SOURCE_DIR}/SharedPtr.h)

set(CORELIB_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tests.cpp)

set(ROOT ${CMAKE_CURRENT_SOURCE_DIR}/..)

set(GTEST_SOURCE_DIR ${ROOT}/External/googletest)

add_executable(CoreLibTests ${CORELIB_SOURCES} ${CORELIB_HEADERS})

target_link_libraries(CoreLibTests PRIVATE gtest gtest_main)

target_link_libraries(CoreLibTests PRIVATE FuegoInterface)
target_link_libraries(CoreLibTests PRIVATE FuegoEngine) # Linker issue with log

target_include_directories(
  CoreLibTests PRIVATE ${ROOT}/External/googletest/googletest/include)

add_test(NAME CoreLibTests COMMAND ${OUTPUT_DIR}/$<CONFIG>/CoreLibTests)
