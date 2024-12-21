# Renderer dependencies

set( PROJECT_HEADERS
    Buffer.h
    CommandBuffer.h
    CommandPool.h
    CommandQueue.h
    DescriptorBuffer.h
    Device.h
    Sampler.h
    Shader.h
    Surface.h
    Swapchain.h
    Texture.h
    Renderer.h
)

set( PROJECT_SOURCES Renderer.cpp)

set(OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../build/win/output)

add_library( Renderer STATIC ${PROJECT_HEADERS} ${PROJECT_SOURCES} )

target_include_directories( Renderer PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../Windows
)

set_target_properties(Renderer PROPERTIES LINKER_LANGUAGE CXX)

set_target_properties(Renderer PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR}
  LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR}
  RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}
)

target_link_libraries( Renderer PRIVATE FuegoInterface)
target_link_libraries( Renderer PRIVATE WindowsDep )