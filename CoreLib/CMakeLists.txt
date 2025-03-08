# CoreLib

set(CMAKE_CXX_STANDARD 20)

set(ROOT ${CMAKE_CURRENT_SOURCE_DIR})

 set(CORELIB_HEADERS
     ${ROOT}/CoreLibConcepts.h
     ${ROOT}/Uptr.h 
     ${ROOT}/Sptr.h
     ${ROOT}/DefaultAllocator.h
     ${ROOT}/CoreLibTests.h)

set(CORELIB_SOURCES ${ROOT}/CoreLibTests.cpp)
set(CORELIB_BULD ${ROOT}/../build/${PLATFORM}/CoreLibTests)

set(OUTPUT_DIR ${CORELIB_BULD}/ )

add_library(CoreLibTests STATIC ${CORELIB_SOURCES} ${CORELIB_HEADERS})

target_link_libraries(CoreLibTests PUBLIC GTest::gtest
                                          FuegoInterface)

target_include_directories(CoreLibTests PUBLIC ${ROOT}/../External/googletest/googletest/include)
target_include_directories(CoreLibTests PUBLIC ${GOOGLE_TEST_ROOT}/googletest/include)