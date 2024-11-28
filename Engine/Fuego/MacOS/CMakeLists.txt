# MacOS dependencies

set(PROJECT_HEADERS
    BufferMetal.h
    EventQueueMacOS.h
    GameViewController.h
    InputMacOS.h
    KeyCodesMacOS.h
    Renderer.h
    ShaderTypes.h
    WindowMacOS.h
    Metal/MetalContext.h)

set(PROJECT_SOURCES
    BufferMetal.cpp
    InputMacOS.cpp
    EventQueueMacOS.mm
    GameViewController.mm
    Renderer.mm
    WindowMacOS.mm
    Metal/MetalContext.mm
    Metal/MetalImplementation.cpp)

set(OUTPUT_DIR ${CMAKE_BINARY_DIR}/../../build/win/output)

add_library(MacOSDep STATIC ${PROJECT_SOURCES} ${PROJECT_HEADERS})

target_link_libraries(MacOSDep PRIVATE FuegoInterface)
