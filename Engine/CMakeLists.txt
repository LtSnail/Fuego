# Engine

include(ExternalProject)

set(FUEGO_DYNAMIC_LIB
    ON
    CACHE BOOL "Build dynamic library (ON/OFF)")

include(${CMAKE_CURRENT_SOURCE_DIR}/ImGui.cmake)

#set(ABSEIL_BUILD_DIR ${CMAKE_BINARY_DIR}/Engine/External/Abseil_Build_Folder)
#set(ABSEIL_INSTALL_DIR ${CMAKE_BINARY_DIR}/install/abseil )
# -- Abseil configuration
#set(ABSL_BUILD_TESTING OFF CACHE BOOL "Enable Abseil testing")
#set(ABSL_USE_GOOGLETEST_HEAD OFF CACHE BOOL "Use the latest GoogleTest")
#set(CMAKE_CXX_STANDARD 20 CACHE STRING "C++ standard")
#set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)
#set(THREADS_PREFER_PTHREAD_FLAG OFF)
# set(PATH_TO_ABSEIL ${CMAKE_CURRENT_SOURCE_DIR}/External/abseil/ )
# set(CMAKE_INSTALL_PREFIX "C:/Work/Fuego/build/win/output/Debug" CACHE STRING "Install directory")
# set(CMAKE_PREFIX_PATH "C:/Work/Fuego/build/win/output/Debug" CACHE STRING "Install directory")
# find_package(absl REQUIRED)
# -- End Abseil configuration

# ExternalProject_Add(
#     abseil
#     PREFIX "${ABSEIL_BUILD_DIR}/Abseil_Temp"
#     SOURCE_DIR "${ABSEIL_BUILD_DIR}/../../../../../Engine/External/abseil"
#     CMAKE_ARGS
#         -DCMAKE_INSTALL_PREFIX=${ABSEIL_BUILD_DIR}/Install
#         -DABSL_BUILD_TESTING=OFF
#         -DABSL_USE_GOOGLETEST_HEAD=OFF
#         -DCMAKE_CXX_STANDARD=20
#     BUILD_BYPRODUCTS "${ABSEIL_BUILD_DIR}/Install/lib/cmake/absl/abslConfig.cmake"
# )
# set(CMAKE_PREFIX_PATH "${ABSEIL_BUILD_DIR}/Install/abseil" CACHE STRING "Path to installed dependencies" FORCE)



# -- Protobuf configuration
#set(PATH_TO_PROTOBUF ${CMAKE_CURRENT_SOURCE_DIR}/Engine/External/protobuf )
#set(CMAKE_PREFIX_PATH ${OUTPUT_DIR})
#set(protobuf_BUILD_TESTS OFF CACHE BOOL "Disable protobuf tests" FORCE)
#set(protobuf_USE_EXTERNAL_GTEST OFF CACHE BOOL "Disable external GTest usage in protobuf" FORCE)
#set(BUILD_TESTING OFF CACHE BOOL "Disable all tests" FORCE)
# -- End Protobuf configuration --

# Project set up
set(PROJECT_HEADERS
    Fuego/Application.h
    Fuego/Core.h
    Fuego/EntryPoint.h
    Fuego/Log.h
    Fuego/Window.h
    Fuego/EventQueue.h
    Fuego/Events/Event.h
    Fuego/Events/EventVisitor.h
    Fuego/Events/KeyEvent.h
    Fuego/Events/MouseEvent.h
    Fuego/Events/ApplicationEvent.h
    Fuego/MouseCodes.h
    Fuego/KeyCodes.h
    Fuego/Layer.h
    Fuego/Input.h
    Fuego/LayerStack.h
    Fuego/ImGui/ImGuiLayer.h
    Fuego/Renderer
    Fuego/FileSystem/FileSystem.h
    External/stb_image/stb_image.h
    Fuego.h
    fupch.h
)

set(SOURCES Fuego/Application.cpp Fuego/Log.cpp Fuego/Layer.cpp
            Fuego/LayerStack.cpp Fuego/ImGui/ImGuiLayer.cpp Fuego/FileSystem/FileSystem.cpp External/stb_image/stb_image.cpp)

set(RENDERER_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Fuego/Renderer )

if(FUEGO_DYNAMIC_LIB)
  set(LIB_TYPE SHARED)
  set(DLL_COMPILE_DEF FUEGO_DYNAMIC_LIB)
else()
  set(LIB_TYPE STATIC)
endif()

# Common setup
list(APPEND ENGINE_DEPENDENCIES ImGui)
list(APPEND ENGINE_DEPENDENCIES Renderer)

set(PUBLIC_ENGINE_COMPILE_DEFS FUEGO_ENABLE_ASSERTS)

set(PPIVATE_ENGINE_COMPILE_DEFS ${DLL_COMPILE_DEF} FUEGO_BUILD_LIB)

set(PUBLIC_ENGINE_COMPILE_FEATURES cxx_std_20)

if(MSVC)
  set(PRIVATE_ENGINE_COMPILE_OPTIONS /W4 /WX /permissive-)
  set(PUBLIC_ENGINE_COMPILE_OPTIONS /utf-8)
else()
  set(PRIVATE_ENGINE_COMPILE_OPTIONS -Wall -Wextra -Werror)
endif()

# End of common
if(WIN32)
  list(APPEND ENGINE_DEPENDENCIES WindowsDep)
  set(PLATFORM_SUBDIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/Fuego/Windows)
elseif(APPLE)
  list(APPEND ENGINE_DEPENDENCIES MacOSDep)
  set(PLATFORM_SUBDIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/Fuego/MacOS)
endif()
# End Project set up

add_subdirectory( ${PLATFORM_SUBDIRECTORIES} )
add_subdirectory( ${RENDERER_PATH} )
#add_subdirectory(${CMAKE_SOURCE_DIR}/Engine/External/protobuf)
#add_dependencies(protobuf abseil)
set(ENGINE_PUBLIC_INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Fuego
    ${CMAKE_CURRENT_SOURCE_DIR}/Fuego/Renderer 
    ${CMAKE_CURRENT_SOURCE_DIR}/External/glm
    ${CMAKE_CURRENT_SOURCE_DIR}/External/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/External/spdlog/include 
)

# An interface library to propagate headers to platform implementations
add_library(FuegoInterface INTERFACE)
target_include_directories(FuegoInterface
                           INTERFACE ${ENGINE_PUBLIC_INCLUDE_DIRECTORIES})
target_precompile_headers(FuegoInterface INTERFACE
                          ${CMAKE_CURRENT_SOURCE_DIR}/fupch.h)
target_compile_features(FuegoInterface
                        INTERFACE ${PUBLIC_ENGINE_COMPILE_FEATURES})
target_compile_definitions(
  FuegoInterface INTERFACE ${PUBLIC_ENGINE_COMPILE_DEFS} FUEGO_BUILD_LIB)
target_compile_options(FuegoInterface
                       INTERFACE ${PUBLIC_ENGINE_COMPILE_OPTIONS})
# Uncomment the line bellow after CoreLib refactor
# target_compile_options(FuegoInterface INTERFACE
# ${PRIVATE_ENGINE_COMPILE_OPTIONS})
set(CMAKE_PREFIX_PATH "C:\\Work\\Fuego\\build\\win\\output\\Debug" CACHE STRING "Path to installed dependencies" FORCE)
find_package(Protobuf)
# An engine library to be used by clients
add_library(FuegoEngine ${LIB_TYPE} ${SOURCES} ${PROJECT_HEADERS})
find_package(absl REQUIRED)
target_link_libraries(FuegoEngine PUBLIC ${ENGINE_DEPENDENCIES} Renderer )
target_link_libraries(FuegoEngine PUBLIC
   absl::strings
   absl::base
   absl::hash
   absl::status
   absl::synchronization
   absl::flat_hash_map
   absl::inlined_vector
   absl::time
   absl::optional
   absl::span
   absl::cord
   absl::log
)
#find_package(protobuf REQUIRED)
target_link_libraries(FuegoEngine PRIVATE protobuf::libprotoc)
target_link_libraries(FuegoEngine PRIVATE protobuf::libprotobuf-lite)
target_link_libraries(FuegoEngine PRIVATE protobuf::libprotobuf)

#target_include_directories(FuegoEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../build/win/Engine/External/abseil/Installed/include)
#message(STATUS "Abseil Include Path: ${CMAKE_CURRENT_SOURCE_DIR}/../build/win/Engine/External/abseil/Installed/include")

#include_directories(${Protobuf_INCLUDE_DIRS})

#target_include_directories(FuegoEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../build/win/Engine/External/abseil/Installed/include)
#message(STATUS "Protobuf Include Path: ${CMAKE_CURRENT_SOURCE_DIR}/../build/win/Engine/External/protobuf/upb/reflection/cmake/google/protobuf")

target_include_directories(FuegoEngine PUBLIC ${ENGINE_PUBLIC_INCLUDE_DIRECTORIES})

#target_include_directories(FuegoEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/build/win/Engine/External/abseil/Installed/lib)

#target_include_directories(FuegoEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/build/win/Engine/External/abseil/Installed/include/absl)
#target_include_directories(FuegoEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/build/win/Engine/External/abseil/Installed/include)

target_precompile_headers(FuegoEngine PUBLIC
                          ${CMAKE_CURRENT_SOURCE_DIR}/fupch.h)

target_compile_definitions( FuegoEngine PUBLIC  ${PUBLIC_ENGINE_COMPILE_DEFS}     )
target_compile_definitions( FuegoEngine PRIVATE ${PPIVATE_ENGINE_COMPILE_DEFS}    )
target_compile_features   ( FuegoEngine PUBLIC  ${PUBLIC_ENGINE_COMPILE_FEATURES} )
target_compile_features   ( FuegoEngine PRIVATE ${PRIVATE_ENGINE_COMPILE_FEATURES})
target_compile_options    ( FuegoEngine PUBLIC  ${PUBLIC_ENGINE_COMPILE_OPTIONS}  )
target_compile_options    ( FuegoEngine PRIVATE ${PRIVATE_ENGINE_COMPILE_OPTIONS} )
