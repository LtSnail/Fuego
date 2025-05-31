# === MacOS Dependencies ===

# === Source Files ===
SET(PROJECT_HEADERS
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
    Metal/SwapchainMetal.h
)

SET(PROJECT_SOURCES
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
    Metal/SwapchainMetal.mm
)

# === Output Directory ===
SET(OUTPUT_DIR ${CMAKE_BINARY_DIR}/output)
FILE(MAKE_DIRECTORY ${OUTPUT_DIR})

# === Metal Dependencies ===
FIND_LIBRARY(APPKIT AppKit REQUIRED)
FIND_LIBRARY(METAL Metal REQUIRED)
FIND_LIBRARY(METALKIT MetalKit REQUIRED)
FIND_LIBRARY(MODELIO ModelIO REQUIRED)
FIND_LIBRARY(QUARTZCORE QuartzCore REQUIRED)

# === MacOSDep Target ===
ADD_LIBRARY(MacOSDep STATIC ${PROJECT_SOURCES} ${PROJECT_HEADERS})

# === Target Configuration ===
TARGET_COMPILE_FEATURES(MacOSDep PUBLIC cxx_std_20)

TARGET_COMPILE_OPTIONS(MacOSDep PRIVATE -fobjc-arc)

TARGET_INCLUDE_DIRECTORIES(
  MacOSDep
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../External/metal-cpp
)

TARGET_LINK_LIBRARIES(
  MacOSDep
  PUBLIC ${APPKIT} ${METAL} ${METALKIT} ${MODELIO} ${QUARTZCORE}
  PRIVATE FuegoInterface
)

# === IDE Organization ===
SET_TARGET_PROPERTIES(
  MacOSDep
  PROPERTIES FOLDER "Platform/MacOS"
             ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR}
             LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR}
             RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}
)
