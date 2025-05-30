# Windows dependencies

set(GLAD_BUILD_DIR ${CMAKE_BINARY_DIR}/Glad)
file(REAL_PATH ${CMAKE_BINARY_DIR}/../../build/win/output OUTPUT_DIR)
file(REAL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../External/glad GLAD_PATH)

set(PROJECT_HEADERS
    EventQueueWin.h
    InputWin.h
    KeyCodesWin.h
    WindowWin.h
    FileSystemPathsWin.h
    OpenGL/BufferOpenGL.h
    OpenGL/DeviceOpenGL.h
    OpenGL/CommandBufferOpenGL.h
    OpenGL/ShaderOpenGL.h
    OpenGL/CommandQueueOpenGL.h
    OpenGL/SwapchainOpenGL.h
    OpenGL/CommandPoolOpenGL.h
    OpenGL/TextureOpenGL.h
    OpenGL/SurfaceOpenGL.h
    OpenGl/MaterialOpenGL.h
    OpenGl/ShaderObjectOpenGL.h
    TimeWin.h
)

set(PROJECT_SOURCES
    EventQueueWin.cpp
    InputWin.cpp
    WindowWin.cpp
    EntryPointWindows.cpp
    OpenGL/BufferOpenGL.cpp
    OpenGL/DeviceOpenGL.cpp
    OpenGL/CommandBufferOpenGL.cpp
    OpenGL/ShaderOpenGL.cpp
    OpenGL/CommandQueueOpenGL.cpp
    OpenGL/SwapchainOpenGL.cpp
    OpenGL/CommandPoolOpenGL.cpp
    OpenGL/TextureOpenGL.cpp
    OpenGL/SurfaceOpenGL.cpp
    OpenGL/MaterialOpenGL.cpp
    OpenGL/ShaderObjectOpenGL.cpp
    TimeWin.cpp
)

add_subdirectory(${GLAD_PATH} ${GLAD_BUILD_DIR})

add_library(WindowsDep STATIC ${PROJECT_SOURCES} ${PROJECT_HEADERS})

target_include_directories(WindowsDep PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${GLAD_PATH}/include
)

target_link_libraries(WindowsDep 
        PUBLIC Glad
        PRIVATE FuegoInterface
)

set_target_properties(Glad PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR}
        LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR}
        RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}
        FOLDER "3rd party"
)