# MacOS dependencies

set(PROJECT_HEADERS
    BufferMetal.h
    EventQueueMacOS.h
    InputMacOS.h
    KeyCodesMacOS.h
    WindowMacOS.h
    Metal/MetalContext.h)

set(PROJECT_SOURCES
    BufferMetal.cpp
    InputMacOS.cpp
    EventQueueMacOS.mm
    WindowMacOS.mm
    Metal/MetalContext.mm
    Metal/MetalImplementation.cpp)

set(OUTPUT_DIR ${CMAKE_BINARY_DIR}/../../build/win/output)

set(PRIVATE_ENGINE_INCLUDE_DIRECTORIES External/metal-cpp)

add_library(MacOSDep STATIC ${PROJECT_SOURCES} ${PROJECT_HEADERS})

target_include_directories(MacOSDep PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(MacOSDep PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../External/metal-cpp)

list(APPEND ENGINE_SHADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/Fuego/MacOS/Metal/Triangle.metal
    PARENT_SCOPE)

find_library(APPKIT AppKit)
find_library(METAL Metal)
find_library(METALKIT MetalKit)
find_library(MODELIO ModelIO)
find_library(QUARTZCORE QuartzCore)

list(APPEND DEPENDENCIES
    ${APPKIT}
    ${METAL}
    ${METALKIT}
    ${MODELIO}
    ${GAMECONTROLLER}
    ${QUARTZCORE})

target_compile_options(MacOSDep PRIVATE -fobjc-arc)

target_link_libraries(MacOSDep PUBLIC ${DEPENDENCIES})
target_link_libraries(MacOSDep PRIVATE FuegoInterface)
