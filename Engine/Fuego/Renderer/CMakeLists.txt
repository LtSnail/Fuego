# Renderer dependencies

# Assimp
file(REAL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../External/assimp ASSIMP_PATH)
set(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_COVERALLS OFF)
set(ASSIMP_BUILD_ZLIB ON)
set(ASSIMP_HUNTER_ENABLED OFF)
# End of Assimp

set(PUBLIC_INCLUDE_DIRECTORIES ${ASSIMP_PATH}/include)
set(PRIVATE_INCLUDE_DIRECTORIES)
set(PUBLIC_LINK_LIBRARIES)
set(PRIVATE_LINK_LIBRARIES FuegoInterface assimp::assimp)

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
    Model.h
    Camera.h
    Material.h
    ShaderObject.h
    VertexLayout.h
    Graphics.hpp
    Image2D.h
    Color.h)

set(PROJECT_SOURCES Renderer.cpp Model.cpp Camera.cpp VertexLayout.cpp Image2D.cpp Color.cpp)

if (FUEGO_PLATFORM_WIN)
    list(append PRIVATE_LINK_LIBRARIES WindowsDep)
    list(append PUBLIC_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}
         ${CMAKE_CURRENT_SOURCE_DIR}/../Windows)

elseif (FUEGO_PLATFORM_MACOS)
    list(append PRIVATE_LINK_LIBRARIES MacOSDep)
    list(append PUBLIC_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}
         ${CMAKE_CURRENT_SOURCE_DIR}/../MacOS)
endif ()

add_subdirectory(${ASSIMP_PATH} ${ASSIMP_PATH})

add_library(Renderer static ${PROJECT_HEADERS} ${PROJECT_SOURCES})

set_target_properties(
    Renderer
    properties FOLDER "Engine/Renderer"
               ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR}
               LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR}
               RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})

target_link_libraries(
    Renderer
    private ${PRIVATE_LINK_LIBRARIES}
    public ${PUBLIC_LINK_LIBRARIES})

target_include_directories(
    Renderer
    private ${PRIVATE_INCLUDE_DIRECTORIES}
    public ${PUBLIC_INCLUDE_DIRECTORIES})

# Set properties only on existing targets
if (target assimp)
    set_target_properties(assimp properties FOLDER "External/Assimp")
endif ()

if (target zlibstatic)
    set_target_properties(zlibstatic properties FOLDER "External/Assimp")
endif ()
