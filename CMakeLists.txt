cmake_minimum_required(VERSION 3.24)

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

add_subdirectory(Engine)
add_subdirectory(Sandbox)
add_subdirectory(CoreLib)

add_subdirectory(External/googletest)
