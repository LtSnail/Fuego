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
    Mesh.h
    Camera.h
    Material.h
    ShaderObject.h
    VertexLayout.h
    ../../Editor/Scene.h)

set(PROJECT_SOURCES Renderer.cpp Mesh.cpp Camera.cpp VertexLayout.cpp ../../Editor/Scene.cpp)

add_library(Renderer STATIC ${PROJECT_HEADERS} ${PROJECT_SOURCES})

if(WIN32)
  target_include_directories(
    Renderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}/../Windows)
target_link_libraries(Renderer PRIVATE WindowsDep)

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

