# Engine

set(FUEGO_DYNAMIC_LIB
    ON
    CACHE BOOL "Build dynamic library (ON/OFF)")

include(${CMAKE_CURRENT_SOURCE_DIR}/ImGui.cmake)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL" CACHE STRING "" FORCE)

# -- Abseil configuration
set(ABSL_BUILD_TESTING OFF CACHE BOOL "Enable Abseil testing")
set(ABSL_USE_GOOGLETEST_HEAD OFF CACHE BOOL "Use the latest GoogleTest")
set(CMAKE_CXX_STANDARD 20 CACHE STRING "C++ standard")
set(PATH_TO_ABSEIL ${CMAKE_CURRENT_SOURCE_DIR}/External/abseil )
# -- End Abseil configuration

# -- Protobuf configuration
set(PATH_TO_PROTOBUF ${CMAKE_CURRENT_SOURCE_DIR}/External/protobuf )
set(CMAKE_PREFIX_PATH ${OUTPUT_DIR})
set(protobuf_BUILD_TESTS OFF CACHE BOOL "Disable protobuf tests" FORCE)
set(protobuf_USE_EXTERNAL_GTEST OFF CACHE BOOL "Disable external GTest usage in protobuf" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "Disable all tests" FORCE)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL" CACHE STRING "" FORCE)
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
add_subdirectory( ${PATH_TO_ABSEIL} )
add_subdirectory( ${PATH_TO_PROTOBUF} )

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

# An engine library to be used by clients
add_library(FuegoEngine ${LIB_TYPE} ${SOURCES} ${PROJECT_HEADERS})

target_link_libraries(FuegoEngine PUBLIC ${ENGINE_DEPENDENCIES} Renderer )
target_link_libraries(FuegoEngine PRIVATE
    absl::strings
    absl::status
    absl::synchronization
    absl::flat_hash_map
    absl::inlined_vector
    absl::time
    absl::hash
    absl::optional
    absl::span
    absl::cord
)
target_link_libraries(FuegoEngine PRIVATE protobuf::libprotoc)
target_link_libraries(FuegoEngine PRIVATE protobuf::libprotobuf)

target_include_directories(FuegoEngine
                           PUBLIC ${ENGINE_PUBLIC_INCLUDE_DIRECTORIES})

target_precompile_headers(FuegoEngine PUBLIC
                          ${CMAKE_CURRENT_SOURCE_DIR}/fupch.h)

target_compile_definitions( FuegoEngine PUBLIC  ${PUBLIC_ENGINE_COMPILE_DEFS}     )
target_compile_definitions( FuegoEngine PRIVATE ${PPIVATE_ENGINE_COMPILE_DEFS}    )
target_compile_features   ( FuegoEngine PUBLIC  ${PUBLIC_ENGINE_COMPILE_FEATURES} )
target_compile_features   ( FuegoEngine PRIVATE ${PRIVATE_ENGINE_COMPILE_FEATURES})
target_compile_options    ( FuegoEngine PUBLIC  ${PUBLIC_ENGINE_COMPILE_OPTIONS}  )
target_compile_options    ( FuegoEngine PRIVATE ${PRIVATE_ENGINE_COMPILE_OPTIONS} )
