# CoreLib
set(CMAKE_CXX_STANDARD 20)

set( CORELIB_HEADERS 
	${CMAKE_CURRENT_SOURCE_DIR}/UniquePtr.h
)

set( CORELIB_SOURCES 
	${CMAKE_CURRENT_SOURCE_DIR}/tests.cpp
)

set( ROOT ${CMAKE_CURRENT_SOURCE_DIR}/.. )

add_executable(CoreLibTests ${CORELIB_SOURCES} ${CORELIB_HEADERS})

if ( APPLE )
	target_compile_definitions( CoreLibTests PUBLIC FUEGO_PLATFORM_MACOS )
elseif( WIN32 )
	target_compile_definitions( CoreLibTests PUBLIC FUEGO_PLATFORM_WIN )
endif()

target_include_directories(CoreLibTests PRIVATE
    ${ROOT}
)