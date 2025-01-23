# MacOS dependencies

set(PROJECT_HEADERS
    EventQueueMacOS.h
    InputMacOS.h
    KeyCodesMacOS.h
    WindowMacOS.h
    FileSystemPathsMacOS.h
    Metal/DeviceMetal.h
    Metal/BufferMetal.h
    Metal/CommandQueueMetal.h
    Metal/ShaderMetal.h
    Metal/CommandBufferMetal.h
    Metal/TextureMetal.h
    Metal/SurfaceMetal.h
    Metal/CommandPoolMetal.h
    Metal/SwapchainMetal.h)

set(PROJECT_SOURCES
    InputMacOS.cpp
    EventQueueMacOS.mm
    WindowMacOS.mm
    EntryPointMacOS.mm
    Metal/MetalImplementation.cpp
    Metal/DeviceMetal.mm
    Metal/BufferMetal.mm
    Metal/CommandQueueMetal.mm
    Metal/ShaderMetal.mm
    Metal/CommandBufferMetal.mm
    Metal/TextureMetal.mm
    Metal/SurfaceMetal.mm
    Metal/CommandPoolMetal.mm
    Metal/SwapchainMetal.mm)

set(OUTPUT_DIR ${CMAKE_BINARY_DIR}/../../build/win/output)

set(PRIVATE_ENGINE_INCLUDE_DIRECTORIES External/metal-cpp)

add_library(MacOSDep STATIC ${PROJECT_SOURCES} ${PROJECT_HEADERS})

target_include_directories(MacOSDep PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(
  MacOSDep PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../External/metal-cpp)

find_library(APPKIT AppKit)
find_library(METAL Metal)
find_library(METALKIT MetalKit)
find_library(MODELIO ModelIO)
find_library(QUARTZCORE QuartzCore)

list(
  APPEND
  DEPENDENCIES
  ${APPKIT}
  ${METAL}
  ${METALKIT}
  ${MODELIO}
  ${GAMECONTROLLER}
  ${QUARTZCORE})

target_compile_options(MacOSDep PRIVATE -fobjc-arc)

target_link_libraries(MacOSDep PUBLIC ${DEPENDENCIES})
target_link_libraries(MacOSDep PRIVATE FuegoInterface)
