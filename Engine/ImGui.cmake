# === ImGui Library Configuration ===

# === Core ImGui Files ===
SET(IMGUI_CORE_HEADERS External/imgui/imgui.h External/imgui/imconfig.h External/imgui/imgui_internal.h
                       External/imgui/imstb_rectpack.h External/imgui/imstb_textedit.h External/imgui/imstb_truetype.h
)

SET(IMGUI_CORE_SOURCES External/imgui/imgui.cpp External/imgui/imgui_demo.cpp External/imgui/imgui_draw.cpp
                       External/imgui/imgui_tables.cpp External/imgui/imgui_widgets.cpp
)

# === Platform-specific Files and Dependencies ===
SET(IMGUI_PLATFORM_HEADERS "")
SET(IMGUI_PLATFORM_SOURCES "")
SET(IMGUI_PLATFORM_DEPENDENCIES "")

IF(FUEGO_PLATFORM STREQUAL "macos")
  # macOS Backend
  LIST(APPEND IMGUI_PLATFORM_HEADERS External/imgui/backends/imgui_impl_osx.h
       External/imgui/backends/imgui_impl_metal.h
  )

  LIST(APPEND IMGUI_PLATFORM_SOURCES External/imgui/backends/imgui_impl_osx.mm
       External/imgui/backends/imgui_impl_metal.mm
  )

  # macOS Frameworks
  FIND_LIBRARY(APPKIT_FRAMEWORK AppKit REQUIRED)
  FIND_LIBRARY(METAL_FRAMEWORK Metal REQUIRED)
  FIND_LIBRARY(METALKIT_FRAMEWORK MetalKit REQUIRED)
  FIND_LIBRARY(MODELIO_FRAMEWORK ModelIO REQUIRED)
  FIND_LIBRARY(GAMECONTROLLER_FRAMEWORK GameController REQUIRED)

  LIST(
    APPEND
    IMGUI_PLATFORM_DEPENDENCIES
    ${APPKIT_FRAMEWORK}
    ${METAL_FRAMEWORK}
    ${METALKIT_FRAMEWORK}
    ${MODELIO_FRAMEWORK}
    ${GAMECONTROLLER_FRAMEWORK}
  )

ELSEIF(FUEGO_PLATFORM STREQUAL "win")
  # Windows Backend
  LIST(APPEND IMGUI_PLATFORM_HEADERS External/imgui/backends/imgui_impl_win32.h
       External/imgui/backends/imgui_impl_opengl3.h
  )

  LIST(APPEND IMGUI_PLATFORM_SOURCES External/imgui/backends/imgui_impl_win32.cpp
       External/imgui/backends/imgui_impl_opengl3.cpp
  )

  # Windows will link OpenGL libraries through glad/other mechanisms

ENDIF()

# === ImGui Library Target ===
ADD_LIBRARY(
  ImGui STATIC ${IMGUI_CORE_SOURCES} ${IMGUI_CORE_HEADERS} ${IMGUI_PLATFORM_SOURCES} ${IMGUI_PLATFORM_HEADERS}
)

# === Target Configuration ===
TARGET_COMPILE_FEATURES(ImGui PRIVATE cxx_std_${FU_CPP_LANG_VER})

TARGET_INCLUDE_DIRECTORIES(ImGui PUBLIC External/imgui/ External/imgui/backends/)

TARGET_LINK_LIBRARIES(ImGui PUBLIC ${IMGUI_PLATFORM_DEPENDENCIES})

# === Compiler-specific Options ===
IF(MSVC)
  TARGET_COMPILE_OPTIONS(
    ImGui PRIVATE /W3 # Lower warning level for third-party code
                  /wd4996 # Disable deprecated warnings
  )
ELSE()
  TARGET_COMPILE_OPTIONS(
    ImGui PRIVATE -Wall -Wno-unused-parameter # ImGui has many unused parameters
                  -Wno-deprecated-declarations
  )
ENDIF()

# === Platform-specific Configuration ===
IF(FUEGO_PLATFORM STREQUAL "macos")
  # macOS-specific compile definitions
  TARGET_COMPILE_DEFINITIONS(ImGui PRIVATE IMGUI_IMPL_METAL_CPP)

  # Enable Objective-C++ for .mm files
  SET_SOURCE_FILES_PROPERTIES(External/imgui/backends/imgui_impl_osx.mm PROPERTIES COMPILE_FLAGS "-fobjc-arc")

ELSEIF(FUEGO_PLATFORM STREQUAL "win")
  # Windows-specific compile definitions
  TARGET_COMPILE_DEFINITIONS(ImGui PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD WIN32_LEAN_AND_MEAN NOMINMAX)

  # === Visual Studio Debug Support ===
  # Copy ImGui debug visualization files to build directory
  SET(IMGUI_DEBUG_SOURCE_DIR ${CMAKE_SOURCE_DIR}/Engine/External/imgui/misc/debuggers)
  SET(IMGUI_DEBUG_FILES ${IMGUI_DEBUG_SOURCE_DIR}/imgui.natstepfilter ${IMGUI_DEBUG_SOURCE_DIR}/imgui.natvis)

  SET(IMGUI_DEBUG_DEST_DIR ${CMAKE_BINARY_DIR}/output)

  FOREACH(debug_file IN LISTS IMGUI_DEBUG_FILES)
    GET_FILENAME_COMPONENT(debug_file_name ${debug_file} NAME)
    ADD_CUSTOM_COMMAND(
      TARGET ImGui
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${debug_file} ${IMGUI_DEBUG_DEST_DIR}/${debug_file_name}
      COMMENT "Installing ImGui debug support: ${debug_file_name}"
      VERBATIM
    )
  ENDFOREACH()

ENDIF()

# === Folder Organization ===
SET_TARGET_PROPERTIES(
  ImGui
  PROPERTIES FOLDER "External/ImGui" DEBUG_POSTFIX "_d"
)

# === Additional ImGui Configuration ===
TARGET_COMPILE_DEFINITIONS(ImGui PUBLIC IMGUI_USER_CONFIG="imconfig.h")
