# CoreLib

set(CMAKE_CXX_STANDARD 20)

set(ROOT ${CMAKE_CURRENT_SOURCE_DIR})

 set(CORELIB_HEADERS
     ${ROOT}/CoreLibConcepts.h
     ${ROOT}/Uptr.h 
     ${ROOT}/Sptr.h
     ${ROOT}/DefaultAllocator.h
     ${ROOT}/singleton.hpp
)

set(CORELIB_SOURCES ${ROOT}/DefaultAllocator.cpp)

set(CORELIB_BULD ${ROOT}/../build/${FUEGO_PLATFORM}/CoreLibTests)

set(OUTPUT_DIR ${CORELIB_BULD}/ )

add_library(CoreLib STATIC ${CORELIB_SOURCES} ${CORELIB_HEADERS})

target_link_libraries(CoreLib PUBLIC FuegoInterface)
