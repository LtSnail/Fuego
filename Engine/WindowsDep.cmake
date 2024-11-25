# Windows dependencies

include(${CMAKE_CURRENT_SOURCE_DIR}/Glad.cmake)

set(PROJECT_HEADERS
    Fuego/Windows/EventQueueWin.h Fuego/Windows/InputWin.h
    Fuego/Windows/KeyCodesWin.h Fuego/Windows/WindowWin.h
    Fuego/Windows/OpenGL/BufferOpenGL.h Fuego/Windows/OpenGL/OpenGLContext.h)

set(PROJECT_SOURCES
    Fuego/Windows/EventQueueWin.cpp Fuego/Windows/InputWin.cpp
    Fuego/Windows/WindowWin.cpp Fuego/Windows/OpenGL/BufferOpenGL.cpp
    Fuego/Windows/OpenGL/OpenGLContext.cpp)

set(DEPENDENCIES Glad)

add_library(WindowsDep STATIC ${PROJECT_SOURCES} ${PROJECT_HEADERS})

target_compile_features(WindowsDep PRIVATE cxx_std_20)

target_compile_definitions(WindowsDep PUBLIC FUEGO_PLATFORM_WIN)
target_compile_definitions(WindowsDep PRIVATE FUEGO_BUILD_LIB)
target_compile_definitions(WindowsDep PUBLIC FUEGO_ENABLE_ASSERTS)

target_compile_options(WindowsDep PUBLIC /utf-8)

target_include_directories(
  WindowsDep
  PUBLIC External/glad/include Fuego/ Fuego/Events ${CMAKE_CURRENT_SOURCE_DIR}
         External/spdlog/include External/glad/include)

target_link_libraries(WindowsDep PRIVATE Glad)
