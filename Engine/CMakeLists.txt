# Engine

set(FUEGO_DYNAMIC_LIB
    ON
    CACHE BOOL "Build dynamic library (ON/OFF)")

include(${CMAKE_CURRENT_SOURCE_DIR}/ImGui.cmake)

# Project set up
set(PROJECT_HEADERS
    Fuego/Application.h
    Fuego/Core.h
    Fuego/EntryPoint.h
    Fuego/Log.h
    Fuego/Window.h
    Fuego/EventQueue.h
    Fuego/Events/Event.h
    Fuego/Events/KeyEvent.h
    Fuego/Events/MouseEvent.h
    Fuego/Events/ApplicationEvent.h
    Fuego/MouseCodes.h
    Fuego/KeyCodes.h
    Fuego/Layer.h
    Fuego/Input.h
    Fuego/LayerStack.h
    Fuego/ImGui/ImGuiLayer.h
    Fuego.h
    fupch.h
    Fuego/Renderer/GraphicsContext.h
    Fuego/Renderer/Buffer.h)
set(SOURCES Fuego/Application.cpp Fuego/Log.cpp Fuego/Layer.cpp
            Fuego/LayerStack.cpp Fuego/ImGui/ImGuiLayer.cpp)

if(FUEGO_DYNAMIC_LIB)
  set(LIB_TYPE SHARED)
  set(DLL_COMPILE_DEF FUEGO_DYNAMIC_LIB)
else()
  set(LIB_TYPE STATIC)
endif()

# Common setup
set(PLATFORM_DEPENDENCIES ImGui)
set(PLATFORM_INCLUDES
    External/spdlog/include External/glm External/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/ ${CMAKE_CURRENT_SOURCE_DIR}/Fuego/)

set(PUBLIC_PLATFORM_COMPILE_DEFS FUEGO_ENABLE_ASSERTS)

set(PPIVATE_PLATFORM_COMPILE_DEFS ${DLL_COMPILE_DEF} FUEGO_BUILD_LIB)

set(PUBLIC_PLATFORM_COMPILE_FEATURES cxx_std_20)

if(MSVC)
  set(PRIVATE_PLATFORM_COMPILE_OPTIONS /W4 /WX /permissive-)
else()
  set(PRIVATE_PLATFORM_COMPILE_OPTIONS -Wall -Wextra -Werror)
endif()

# End of common
if(WIN32)
  list(APPEND PLATFORM_DEPENDENCIES WindowsDep)

  set(PLATFORM_SUBDIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/Fuego/Windows)
  list(APPEND PLATFORM_INCLUDES Fuego/Windows)
  list(APPEND PLATFORM_INCLUDES FUEGO_PLATFORM_WIN)
  list(APPEND PUBLIC_PLATFORM_COMPILE_DEFS FUEGO_PLATFORM_WIN)
  list(APPEND PPIVATE_PLATFORM_COMPILE_DEFS)
  list(APPEND PUBLIC_PLATFORM_COMPILE_FEATURES)

  set(PUBLIC_PLATFORM_COMPILE_OPTIONS /utf-8)

  list(APPEND PRIVATE_PLATFORM_COMPILE_OPTIONS)

elseif(APPLE)

  find_library(APPKIT AppKit)
  find_library(METAL Metal)
  find_library(METALKIT MetalKit)
  find_library(MODELIO ModelIO)
  find_library(QUARTZCORE QuartzCore)

  list(
    APPEND
    PLATFORM_DEPENDENCIES
    MacOSDep
    ${APPKIT}
    ${METAL}
    ${METALKIT}
    ${MODELIO}
    ${GAMECONTROLLER}
    ${QUARTZCORE})
  set(ENGINE_SHADERS
      ${CMAKE_CURRENT_SOURCE_DIR}/Fuego/MacOS/Metal/Triangle.metal
      PARENT_SCOPE)
  set(PLATFORM_SUBDIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/Fuego/MacOS)

  list(APPEND PLATFORM_INCLUDES Fuego/MacOS)

  set(PRIVATE_PLATFORM_INCLUDES External/metal-cpp)
  list(APPEND PUBLIC_PLATFORM_COMPILE_DEFS FUEGO_PLATFORM_MACOS)
  list(APPEND PPIVATE_PLATFORM_COMPILE_DEFS)
  list(APPEND PUBLIC_PLATFORM_COMPILE_FEATURES)

  set(PUBLIC_PLATFORM_COMPILE_OPTIONS)
  list(APPEND PRIVATE_PLATFORM_COMPILE_OPTIONS -fobjc-arc)

endif()
# End Project set up

add_subdirectory(${PLATFORM_SUBDIRECTORIES})

add_library(FuegoEngine ${LIB_TYPE} ${SOURCES} ${PROJECT_HEADERS})

target_link_libraries(FuegoEngine PUBLIC ${PLATFORM_DEPENDENCIES})
target_include_directories(FuegoEngine PUBLIC ${PUBLIC_PLATFORM_INCLUDES})
target_include_directories(FuegoEngine PRIVATE ${PRIVATE_PLATFORM_INCLUDES})

target_precompile_headers(FuegoEngine PUBLIC
                          ${CMAKE_CURRENT_SOURCE_DIR}/fupch.h)

if(UNIX AND APPLE)
  message(STATUS "The target platform is MacOS")
  target_compile_definitions(FuegoEngine PUBLIC FUEGO_PLATFORM_MACOS)
  target_compile_options(FuegoEngine PRIVATE -fobjc-arc)
endif()

target_compile_definitions(FuegoEngine PUBLIC ${PUBLIC_PLATFORM_COMPILE_DEFS})
target_compile_definitions(FuegoEngine PRIVATE ${PPIVATE_PLATFORM_COMPILE_DEFS})
target_compile_features(FuegoEngine PUBLIC ${PUBLIC_PLATFORM_COMPILE_FEATURES})
target_compile_options(FuegoEngine PUBLIC ${PUBLIC_PLATFORM_COMPILE_OPTIONS})
target_compile_options(FuegoEngine PRIVATE ${PRIVATE_PLATFORM_COMPILE_OPTIONS})
