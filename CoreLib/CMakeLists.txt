# CoreLib
set( CMAKE_CXX_STANDARD 20 )

set( CORELIB_HEADERS 
    ${CMAKE_CURRENT_SOURCE_DIR}/UniquePtr.h
)

set( CORELIB_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/tests.cpp
)

set( ROOT ${CMAKE_CURRENT_SOURCE_DIR}/.. )

set( GTEST_SOURCE_DIR ${ROOT}/External/googletest )

add_executable(CoreLibTests ${CORELIB_SOURCES} ${CORELIB_HEADERS})

target_link_libraries(CoreLibTests PRIVATE gtest gtest_main)

if ( APPLE )
    target_compile_definitions( CoreLibTests PUBLIC FUEGO_PLATFORM_MACOS )
elseif( WIN32 )
    target_compile_definitions( CoreLibTests PUBLIC FUEGO_PLATFORM_WIN )
endif()

target_compile_definitions( CoreLibTests PUBLIC FUEGO_BUILD_LIB )

target_include_directories( CoreLibTests PRIVATE
    ${ROOT}
	${ROOT}/External/googletest/googletest/include
)