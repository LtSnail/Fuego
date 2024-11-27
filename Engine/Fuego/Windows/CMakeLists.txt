# Windows dependencies

set(PROJECT_HEADERS EventQueueWin.h InputWin.h KeyCodesWin.h WindowWin.h
                    OpenGL/BufferOpenGL.h OpenGL/OpenGLContext.h)

set(PROJECT_SOURCES EventQueueWin.cpp InputWin.cpp WindowWin.cpp
                    OpenGL/BufferOpenGL.cpp OpenGL/OpenGLContext.cpp)

set(GLAD_BUILD_DIR ${CMAKE_BINARY_DIR}/Glad)
set(OUTPUT_DIR ${CMAKE_BINARY_DIR}/../../build/win/output)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../External/glad
                 ${GLAD_BUILD_DIR})

set_target_properties(
  Glad
  PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR}
             LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR}
             RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})

add_library(WindowsDep STATIC ${PROJECT_SOURCES} ${PROJECT_HEADERS})

target_compile_options(WindowsDep PUBLIC /utf-8)

target_include_directories(
  WindowsDep
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../ ${CMAKE_CURRENT_SOURCE_DIR}/../../
         ${CMAKE_CURRENT_SOURCE_DIR}/../../External/spdlog/include
         ${CMAKE_CURRENT_SOURCE_DIR}/../../External/glad/include)

target_precompile_headers(WindowsDep PUBLIC
                          ${CMAKE_CURRENT_SOURCE_DIR}/../../fupch.h)

target_link_libraries(WindowsDep PUBLIC Glad)

target_compile_features(WindowsDep PUBLIC cxx_std_20)
target_compile_definitions(WindowsDep PRIVATE FUEGO_BUILD_LIB)
target_compile_definitions(WindowsDep PRIVATE FUEGO_ENABLE_ASSERTS)
