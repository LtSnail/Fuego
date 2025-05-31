# === Sandbox Application ===

# === Source Files ===
SET(SANDBOX_HEADERS SandboxApp.h)

SET(SANDBOX_SOURCES SandboxApp.cpp)

# === Platform-specific Configuration ===
IF(FUEGO_PLATFORM STREQUAL "macos")
  # macOS Sources
  LIST(APPEND SANDBOX_SOURCES MacOS/main.mm)

  # macOS Resources
  SET(MACOS_RESOURCES Resources/MacOS/Assets.xcassets Resources/MacOS/Base.lproj/Main.storyboard
                      Resources/MacOS/Triangle.metal
  )

  # Create macOS Bundle
  ADD_EXECUTABLE(Sandbox MACOSX_BUNDLE ${SANDBOX_SOURCES} ${SANDBOX_HEADERS} ${MACOS_RESOURCES})

  # Bundle Configuration
  SET_SOURCE_FILES_PROPERTIES(${MACOS_RESOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

  SET_TARGET_PROPERTIES(
    Sandbox
    PROPERTIES XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "${ORGANIZATION}.Sandbox"
               MACOSX_BUNDLE_GUI_IDENTIFIER "${ORGANIZATION}.Sandbox"
               MACOSX_BUNDLE_BUNDLE_NAME "${ORGANIZATION}.Sandbox"
               MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Resources/MacOS/Sandbox-Info.plist.in"
  )

ELSEIF(FUEGO_PLATFORM STREQUAL "win")
  # Windows Sources
  LIST(APPEND SANDBOX_SOURCES Windows/main.cpp)

  # Create Windows Executable
  ADD_EXECUTABLE(Sandbox WIN32 ${SANDBOX_SOURCES} ${SANDBOX_HEADERS})

  # Windows-specific properties
  SET_TARGET_PROPERTIES(Sandbox PROPERTIES WIN32_EXECUTABLE TRUE)
ENDIF()

# === Dependencies ===
TARGET_LINK_LIBRARIES(Sandbox PRIVATE FuegoEngine)

TARGET_COMPILE_FEATURES(Sandbox PRIVATE cxx_std_20)

# === IDE Organization ===
SET_TARGET_PROPERTIES(
  Sandbox
  PROPERTIES FOLDER "Applications" DEBUG_POSTFIX "_d"
)

# === Runtime Directory ===
SET_TARGET_PROPERTIES(
  Sandbox
  PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output
)
