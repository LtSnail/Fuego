# Engine

include(${CMAKE_CURRENT_SOURCE_DIR}/ImGui.cmake)

set( PROJECT_HEADERS 
    Fuego/Application.h
    Fuego/Core.h
    Fuego/EntryPoint.h
    Fuego/Log.h
    Fuego/Window.h
    Fuego/EventQueue.h
    Fuego/Events/Event.h
    Fuego/Events/KeyEvent.h
    Fuego/Events/MouseEvent.h
    Fuego/Events/ApplicationEvent.h
    Fuego/MouseCodes.h
    Fuego/KeyCodes.h
    Fuego/Layer.h
    Fuego/LayerStack.h
    Fuego/ImGui/ImGuiLayer.h
    Fuego.h
    fupch.h
)

set( SOURCES
    Fuego/Application.cpp
    Fuego/Log.cpp
    Fuego/Layer.cpp
    Fuego/LayerStack.cpp
    Fuego/ImGui/ImGuiLayer.cpp
)

set( DEPENDENCIES
    ImGui
)

if ( APPLE )
    set( PROJECT_HEADERS
        ${PROJECT_HEADERS}
        Fuego/MacOS/EventQueueMacOS.h
        Fuego/MacOS/WindowMacOS.h
    )

    set( SOURCES
        ${SOURCES}
        Fuego/MacOS/EventQueueMacOS.mm
        Fuego/MacOS/WindowMacOS.mm
    )

    set( ENGINE_SHADERS 
        ${CMAKE_CURRENT_SOURCE_DIR}/Fuego/MacOS/Shaders.metal
        PARENT_SCOPE
    )

    find_library( APPKIT AppKit )
    find_library( METAL Metal )
    find_library( METALKIT MetalKit )
    find_library( MODELIO ModelIO )

    set( DEPENDENCIES
        ${DEPENDENCIES}
        ${APPKIT}
        ${METAL}
        ${METALKIT}
        ${MODELIO}
        ${GAMECONTROLLER}
    )

elseif ( WIN32 )
    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
        set( PROJECT_HEADERS
            ${PROJECT_HEADERS}
            Fuego/Windows/EventQueueWin.h
            Fuego/Windows/WindowWin.h
            Fuego/Windows/KeyCodesWin.h
        )

        set( SOURCES
            ${SOURCES}
            Fuego/Windows/EventQueueWin.cpp
            Fuego/Windows/WindowWin.cpp
        )

        set( DEPENDENCIES
            ${DEPENDENCIES}
        )

    else()
        message(FATAL_ERROR "Target platform is not X64 - Compilation denied")
    endif()
endif()

add_library( FuegoEngine
    SHARED
    ${SOURCES}
    ${PROJECT_HEADERS}
)

target_link_libraries( FuegoEngine PUBLIC
    ${DEPENDENCIES}
)

target_include_directories( FuegoEngine 
    PUBLIC
    External/spdlog/include
    External/glm
    External/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Fuego
)

if ( WIN32 )
    #GLAD
    set( GLAD_SOURCE_DIR 
        External/glad
    )

    list( APPEND 
        SOURCES ${GLAD_SOURCE_DIR}/src/glad.c
    )

    target_include_directories( FuegoEngine 
        PRIVATE 
        ${GLAD_SOURCE_DIR}/include
    )

endif()

target_precompile_headers( FuegoEngine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/fupch.h )
target_compile_definitions( FuegoEngine PRIVATE FUEGO_BUILD_LIB )
target_compile_features( FuegoEngine PUBLIC cxx_std_20 )

target_compile_definitions( FuegoEngine PUBLIC FUEGO_ENABLE_ASSERTS )

if ( MSVC )
    target_compile_options( FuegoEngine PRIVATE /W4 /WX /permissive- )
    target_compile_options( FuegoEngine PUBLIC /utf-8 )
else()
    # -pedantic
    target_compile_options( FuegoEngine PRIVATE -Wall -Wextra -Werror )
endif()

if( UNIX AND APPLE )
    message( STATUS "The target platform is MacOS" )
    target_compile_definitions( FuegoEngine PUBLIC FUEGO_PLATFORM_MACOS )
    target_compile_options( FuegoEngine PRIVATE -fobjc-arc )
elseif( WIN32 )
    message( STATUS "The target platform is Windows" )
    target_compile_definitions( FuegoEngine PUBLIC FUEGO_PLATFORM_WIN )
endif()
