cmake_minimum_required(VERSION 3.24)

project(
    FuegoSandboxProject
    VERSION 0.1
    DESCRIPTION "Sandbox project"
    LANGUAGES CXX
)

if( CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME )
    set( CMAKE_CXX_EXTENSIONS OFF )
    set_property( GLOBAL PROPERTY USE_FOLDERS ON )
endif()

if ( WIN32 )
    add_definitions(-DUNICODE -D_UNICODE)
endif()

set( ORGANIZATION FUEGO )

add_subdirectory( Engine )
add_subdirectory( Sandbox )
