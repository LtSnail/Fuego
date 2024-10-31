# Engine

set( PROJECT_HEADERS 
    Fuego/Application.h
    Fuego/Core.h
    Fuego/EntryPoint.h
    Fuego/Log.h
    Fuego/EventQueue.h
    Fuego/Events/Event.h
    Fuego/Events/KeyEvent.h
    Fuego/Events/MouseEvent.h
    Fuego/Events/ApplicationEvent.h
    chpch.h
)

set( SOURCES
    Fuego/Application.cpp
    Fuego/Log.cpp
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
        ${APPKIT}
        ${METAL}
        ${METALKIT}
        ${MODELIO}
    )

elseif ( WIN32 )
    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
        set( PROJECT_HEADERS
            ${PROJECT_HEADERS}
            Fuego/Windows/WinEventQueue.h
            Fuego/Windows/WinWindow.h
        )

        set( SOURCES
            ${SOURCES}
            Fuego/Windows/WinEventQueue.cpp
            Fuego/Windows/WinWindow.cpp
        )

    else()
        message(FATAL_ERROR "Target platform is not X64 - Compilation denied")
    endif() # End of Windows
endif() # End of MAC

add_library( FuegoEngine
    SHARED
    ${SOURCES}
    ${PROJECT_HEADERS}
    ${DEPENDENCIES}
)

target_link_libraries( FuegoEngine
    ${DEPENDENCIES}
)

target_include_directories( FuegoEngine 
    PUBLIC
    External/spdlog/include
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Fuego.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Fuego
)

target_precompile_headers( FuegoEngine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/chpch.h )
target_compile_definitions( FuegoEngine PRIVATE FUEGO_BUILD_LIB )
target_compile_features( FuegoEngine PUBLIC cxx_std_20 )

target_compile_definitions( FuegoEngine PUBLIC FUEGO_ENABLE_ASSERTS )

if (MSVC)
    target_compile_options( FuegoEngine PRIVATE /W4 /WX /permissive- )
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
