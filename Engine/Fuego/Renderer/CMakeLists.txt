# Renderer dependencies

set(PUBLIC_INCLUDE_DIRECTORIES)
set(PRIVATE_INCLUDE_DIRECTORIES)
set(PUBLIC_LINK_LIBRARIES)
set(PRIVATE_LINK_LIBRARIES FuegoInterface)

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

if(FUEGO_PLATFORM_WIN)
  list(APPEND PRIVATE_LINK_LIBRARIES WindowsDep)
  list(APPEND PUBLIC_INCLUDE_DIRECTORIES 
      ${CMAKE_CURRENT_SOURCE_DIR}
      ${CMAKE_CURRENT_SOURCE_DIR}/../Windows)

elseif(FUEGO_PLATFORM_MACOS)
  list(APPEND PRIVATE_LINK_LIBRARIES MacOSDep)
  list(APPEND PUBLIC_INCLUDE_DIRECTORIES 
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../MacOS)
endif()


add_library(Renderer STATIC ${PROJECT_HEADERS} ${PROJECT_SOURCES})

set_target_properties(Renderer PROPERTIES 
                      ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR}
                      LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR}
                      RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})

target_link_libraries(Renderer PRIVATE ${PRIVATE_LINK_LIBRARIES})
target_link_libraries(Renderer PUBLIC ${PUBLIC_LINK_LIBRARIES})

target_include_directories(Renderer PRIVATE ${PRIVATE_INCLUDE_DIRECTORIES})
target_include_directories(Renderer PUBLIC ${PUBLIC_INCLUDE_DIRECTORIES})