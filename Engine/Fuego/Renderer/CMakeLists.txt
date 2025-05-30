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
)

set(PROJECT_SOURCES
    Renderer.cpp
    Model.cpp
    Camera.cpp
    VertexLayout.cpp
    Image2D.cpp
)

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

add_subdirectory(${ASSIMP_PATH} ${ASSIMP_PATH})

add_library(Renderer STATIC ${PROJECT_HEADERS} ${PROJECT_SOURCES})

set_target_properties(Renderer PROPERTIES 
                      ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR}
                      LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR}
                      RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}
)

target_link_libraries(Renderer 
        PRIVATE ${PRIVATE_LINK_LIBRARIES}
        PUBLIC ${PUBLIC_LINK_LIBRARIES}
)

target_include_directories(Renderer 
        PRIVATE ${PRIVATE_INCLUDE_DIRECTORIES}
        PUBLIC ${PUBLIC_INCLUDE_DIRECTORIES}
)

set_target_properties(assimp
        UpdateAssimpLibsDebugSymbolsAndDLLs
        uninstall
        zlibstatic
        PROPERTIES FOLDER
        "3rd party/Assimp"
)