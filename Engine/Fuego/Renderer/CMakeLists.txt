# Renderer dependencies

set(PROJECT_HEADERS
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
    Mesh.h)

set(PROJECT_SOURCES Renderer.cpp Mesh.cpp)

add_library(Renderer STATIC ${PROJECT_HEADERS} ${PROJECT_SOURCES})

if(WIN32)
  target_include_directories(
    Renderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}/../Windows)
elseif(APPLE)
  target_include_directories(
    Renderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}/../MacOS)
endif()

set_target_properties(
  Renderer
  PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR}
             LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR}
             RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})

target_link_libraries(Renderer PRIVATE FuegoInterface)
