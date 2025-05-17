# Engine

include(${CMAKE_CURRENT_SOURCE_DIR}/ImGui.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/abseil.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/protobuf.cmake)

set_target_properties(ImGui PROPERTIES FOLDER "3rd party")

# Proto
getprotoheaderfiles(PROTO_OUT ${PROTO_OUTPUT_FOLDER})
getprotosourcefiles(PROTO_OUT ${PROTO_OUTPUT_FOLDER})

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
    ${SCENE_GENERATED_PROTO_HDRS}
    Fuego/Scene.h
    Fuego/Services/ServiceInterfaces.hpp
    Fuego/Services/ServiceLocator.h
    Fuego/ThreadPool.h
    Fuego/FuTime.h)

set(SOURCES
    Fuego/Application.cpp
    Fuego/Log.cpp
    Fuego/Layer.cpp
    Fuego/LayerStack.cpp
    Fuego/ImGui/ImGuiLayer.cpp
    Fuego/FileSystem/FileSystem.cpp
    External/stb_image/stb_image.cpp
    ${SCENE_GENERATED_PROTO_SRCS}
    Fuego/Scene.cpp
    Fuego/Services/ServiceLocator.cpp
    Fuego/ThreadPool.cpp)

set(RENDERER_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Fuego/Renderer)

# Common setup
list(APPEND ENGINE_DEPENDENCIES ImGui)

set(PUBLIC_ENGINE_COMPILE_DEFS FUEGO_ENABLE_ASSERTS FUEGO_BUILD_LIB)

if(FUEGO_LIB_TYPE STREQUAL "SHARED")
  list(APPEND PUBLIC_ENGINE_COMPILE_DEFS FUEGO_DLL_LIB)
endif()

set(PPIVATE_ENGINE_COMPILE_DEFS)
set(_ITERATOR_DEBUG_LEVEL 0)

if(MSVC)
  set(PRIVATE_ENGINE_COMPILE_OPTIONS /W4 /permissive-)
  set(PUBLIC_ENGINE_COMPILE_OPTIONS /utf-8)
else()
  set(PRIVATE_ENGINE_COMPILE_OPTIONS -Wall -Wextra -Werror)
endif()
# End of common

if(FU_CONF_DEBUG)
  set(CMAKE_MSVC_RUNTIME_LIBRARY ${FU_MSVC_RUNTIME_DEBUG})
elseif(FU_CONF_RELEASE)
  set(CMAKE_MSVC_RUNTIME_LIBRARY ${FU_MSVC_RUNTIME_RELEASE})
endif()

if(WIN32)
  list(APPEND ENGINE_DEPENDENCIES WindowsDep)
  set(PLATFORM_SUBDIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/Fuego/Windows)
elseif(APPLE)
  list(APPEND ENGINE_DEPENDENCIES MacOSDep)
  set(PLATFORM_SUBDIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/Fuego/MacOS)
endif()
# End Project set up

add_subdirectory(${PLATFORM_SUBDIRECTORIES})
add_subdirectory(${RENDERER_PATH})

set(ENGINE_PUBLIC_INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../CoreLib
    ${CMAKE_CURRENT_SOURCE_DIR}/Fuego
    ${CMAKE_CURRENT_SOURCE_DIR}/Fuego/ProtoOut
    ${CMAKE_CURRENT_SOURCE_DIR}/Fuego/Renderer
    ${CMAKE_CURRENT_SOURCE_DIR}/External/glm
    ${CMAKE_CURRENT_SOURCE_DIR}/External/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/External/spdlog/include
    ${CMAKE_CURRENT_SOURCE_DIR}/External/protobuf/src)

# An interface library to propagate headers to platform implementations
add_library(FuegoInterface INTERFACE)
target_include_directories(FuegoInterface
                           INTERFACE ${ENGINE_PUBLIC_INCLUDE_DIRECTORIES})
target_precompile_headers(FuegoInterface INTERFACE
                          ${CMAKE_CURRENT_SOURCE_DIR}/fupch.h)
target_compile_features(FuegoInterface
                        INTERFACE ${PUBLIC_ENGINE_COMPILE_FEATURES})
target_compile_definitions(FuegoInterface
                           INTERFACE ${PUBLIC_ENGINE_COMPILE_DEFS})
target_compile_options(FuegoInterface
                       INTERFACE ${PUBLIC_ENGINE_COMPILE_OPTIONS})
# Uncomment the line bellow after CoreLib refactor
# target_compile_options(FuegoInterface INTERFACE
# ${PRIVATE_ENGINE_COMPILE_OPTIONS}) An engine library to be used by clients
add_library(FuegoEngine ${FUEGO_LIB_TYPE} ${SOURCES} ${PROJECT_HEADERS})

add_dependencies(FuegoEngine ProtoGen)

target_link_libraries(FuegoEngine 
    PUBLIC ${ENGINE_DEPENDENCIES} protobuf::libprotobuf
    PRIVATE Renderer CoreLib)

target_include_directories(
  FuegoEngine
  PUBLIC ${ENGINE_PUBLIC_INCLUDE_DIRECTORIES}
  PRIVATE ${Protobuf_INCLUDE_DIRS})

target_precompile_headers(FuegoEngine PUBLIC
                          ${CMAKE_CURRENT_SOURCE_DIR}/fupch.h)
target_compile_definitions(
  FuegoEngine
  PUBLIC ${PUBLIC_ENGINE_COMPILE_DEFS}
  PRIVATE ${PPIVATE_ENGINE_COMPILE_DEFS})
target_compile_features(
  FuegoEngine
  PUBLIC ${PUBLIC_ENGINE_COMPILE_FEATURES}
  PRIVATE ${PRIVATE_ENGINE_COMPILE_FEATURES})
target_compile_options(
  FuegoEngine
  PUBLIC ${PUBLIC_ENGINE_COMPILE_OPTIONS}
  PRIVATE ${PRIVATE_ENGINE_COMPILE_OPTIONS})
