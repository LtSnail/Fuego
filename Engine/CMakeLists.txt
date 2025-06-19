# === Fuego Engine ===

# === External Dependencies ===
INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/assimp.cmake)
INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/ImGui.cmake)
INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/abseil.cmake)
INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/protobuf.cmake)
INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/tbb.cmake)

# === Protocol Buffers Generation ===
GETPROTOHEADERFILES(PROTO_OUT ${PROTO_OUTPUT_FOLDER})
GETPROTOSOURCEFILES(PROTO_OUT ${PROTO_OUTPUT_FOLDER})

# === Source Files ===
SET(ENGINE_HEADERS
    # Core Engine
    Fuego/Application.h
    Fuego/Core.h
    Fuego/EntryPoint.h
    Fuego/Log.h
    Fuego/Window.h
    Fuego/EventQueue.h
    # Event System
    Fuego/Events/Event.h
    Fuego/Events/EventVisitor.h
    Fuego/Events/KeyEvent.h
    Fuego/Events/MouseEvent.h
    Fuego/Events/ApplicationEvent.h
    # Input System
    Fuego/MouseCodes.h
    Fuego/KeyCodes.h
    Fuego/Input.h
    # Layer System
    Fuego/Layer.h
    Fuego/LayerStack.h
    Fuego/ImGui/ImGuiLayer.h
    # Renderer
    Fuego/Renderer
    # File System
    Fuego/FileSystem/FileSystem.h
    # Service System
    Fuego/Services/ServiceInterfaces.hpp
    Fuego/Services/ServiceLocator.h
    # Utilities
    Fuego/ThreadPool.h
    Fuego/FuTime.h
    Fuego/AssetsManager.h
    Fuego/Scene.h
    # Third Party
    External/stb_image/stb_image.h
    # Generated Code
    ${SCENE_GENERATED_PROTO_HDRS}
    # Main Headers
    Fuego.h
    fupch.h
)

SET(ENGINE_SOURCES
    # Core Engine
    Fuego/Application.cpp
    Fuego/Log.cpp
    # Layer System
    Fuego/Layer.cpp
    Fuego/LayerStack.cpp
    Fuego/ImGui/ImGuiLayer.cpp
    # File System
    Fuego/FileSystem/FileSystem.cpp
    # Service System
    Fuego/Services/ServiceLocator.cpp
    # Utilities
    Fuego/ThreadPool.cpp
    Fuego/AssetsManager.cpp
    Fuego/Scene.cpp
    # Third Party
    External/stb_image/stb_image.cpp
    # Generated Code
    ${SCENE_GENERATED_PROTO_SRCS}
    # Implementation Files
)

# === Engine Configuration ===
SET(ENGINE_PUBLIC_DEFINITIONS FUEGO_ENABLE_ASSERTS FUEGO_BUILD_LIB)

# Ensure _ITERATOR_DEBUG_LEVEL is set for all targets
SET(ENGINE_PRIVATE_DEFINITIONS _ITERATOR_DEBUG_LEVEL=${_ITERATOR_DEBUG_LEVEL})

IF(FUEGO_LIB_TYPE STREQUAL "SHARED")
  LIST(APPEND ENGINE_PUBLIC_DEFINITIONS FUEGO_DLL_LIB)
ENDIF()

# === Compiler Options ===
IF(MSVC)
  SET(ENGINE_PRIVATE_COMPILE_OPTIONS /W4 /permissive-)
  SET(ENGINE_PUBLIC_COMPILE_OPTIONS /utf-8)
ELSE()
  SET(ENGINE_PRIVATE_COMPILE_OPTIONS -Wall -Wextra -Werror)
  SET(ENGINE_PUBLIC_COMPILE_OPTIONS "")
ENDIF()

# === Platform Dependencies ===
SET(ENGINE_DEPENDENCIES ImGui TBB::tbb)

IF(FUEGO_PLATFORM STREQUAL "win")
  LIST(APPEND ENGINE_DEPENDENCIES WindowsDep)
  SET(PLATFORM_SUBDIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Fuego/Windows)
ELSEIF(FUEGO_PLATFORM STREQUAL "macos")
  LIST(APPEND ENGINE_DEPENDENCIES MacOSDep)
  SET(PLATFORM_SUBDIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Fuego/MacOS)
ENDIF()

# === Include Directories ===
SET(ENGINE_PUBLIC_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Fuego
    ${CMAKE_CURRENT_SOURCE_DIR}/Fuego/ProtoOut
    ${CMAKE_CURRENT_SOURCE_DIR}/Fuego/Renderer
    ${CMAKE_CURRENT_SOURCE_DIR}/External/glm
    ${CMAKE_CURRENT_SOURCE_DIR}/External/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/External/spdlog/include
    ${CMAKE_CURRENT_SOURCE_DIR}/External/protobuf/src
    ${CMAKE_SOURCE_DIR}/CoreLib
)

# === FuegoInterface - Shared Configuration ===
ADD_LIBRARY(FuegoInterface INTERFACE)

TARGET_INCLUDE_DIRECTORIES(FuegoInterface INTERFACE ${ENGINE_PUBLIC_INCLUDES})

TARGET_PRECOMPILE_HEADERS(FuegoInterface INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/fupch.h)

TARGET_COMPILE_FEATURES(FuegoInterface INTERFACE cxx_std_20)

TARGET_COMPILE_DEFINITIONS(
  FuegoInterface INTERFACE ${ENGINE_PUBLIC_DEFINITIONS} _ITERATOR_DEBUG_LEVEL=${_ITERATOR_DEBUG_LEVEL}
)

TARGET_COMPILE_OPTIONS(FuegoInterface INTERFACE ${ENGINE_PUBLIC_COMPILE_OPTIONS} ${ENGINE_PRIVATE_COMPILE_OPTIONS})

# === Subdirectories ===
ADD_SUBDIRECTORY(${PLATFORM_SUBDIRECTORY})
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/External/tbb)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/Fuego/Renderer)

# === FuegoEngine Target ===
ADD_LIBRARY(FuegoEngine ${FUEGO_LIB_TYPE} ${ENGINE_SOURCES} ${ENGINE_HEADERS})

ADD_DEPENDENCIES(FuegoEngine ProtoGen)

TARGET_LINK_LIBRARIES(
  FuegoEngine
  PUBLIC FuegoInterface ${ENGINE_DEPENDENCIES} protobuf::libprotobuf CoreLib
  PRIVATE Renderer
)

TARGET_INCLUDE_DIRECTORIES(FuegoEngine PRIVATE ${Protobuf_INCLUDE_DIRS})

TARGET_COMPILE_DEFINITIONS(FuegoEngine PRIVATE ${ENGINE_PRIVATE_DEFINITIONS})

TARGET_COMPILE_FEATURES(FuegoEngine PUBLIC cxx_std_20)

# === IDE Organization ===
SET_TARGET_PROPERTIES(
  FuegoEngine
  PROPERTIES FOLDER "Engine" DEBUG_POSTFIX "_d"
)

SET_TARGET_PROPERTIES(
  ImGui
  PROPERTIES FOLDER "External"
)
