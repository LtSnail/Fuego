cmake_minimum_required(VERSION 3.24)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Output folders:
set(OUTPUT_DIR ${CMAKE_BINARY_DIR}/output)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR})

# Convert passed path to cmake style (Unix-style)
file(TO_CMAKE_PATH "${PROTO_PATH}" PROTO_PATH)

if(CMAKE_CONFIGURATION_TYPES MATCHES "Debug")
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebugDLL" CACHE STRING "Runtime Library" FORCE)
elseif(CMAKE_CONFIGURATION_TYPES MATCHES "Release")
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL" CACHE STRING "Runtime Library" FORCE)
endif()

set(CMAKE_BUILD_PARALLEL_LEVEL 8)

project(
  FuegoSandboxProject
  VERSION 0.1
  DESCRIPTION "Sandbox project"
  LANGUAGES CXX)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(CMAKE_CXX_EXTENSIONS OFF)
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

set(ORGANIZATION FUEGO)

enable_testing()

if(APPLE)
  add_compile_definitions(FUEGO_PLATFORM_MACOS)
elseif(WIN32)
  add_compile_definitions(FUEGO_PLATFORM_WIN)
endif()

add_subdirectory(Engine)
add_subdirectory(Sandbox)
if(ENABLE_FUEGO_TEST)
  add_subdirectory(Tests)
endif()