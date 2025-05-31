# --- Fuego Global Configuration --- # This file defines global configuration variables for the Fuego project

# C++ Language standard version
SET(FU_CPP_LANG_VER
    "20"
    CACHE STRING "C++ Language standard version"
)

# Iterator Debug Level configuration
SET(_ITERATOR_DEBUG_LEVEL
    "0"
    CACHE STRING "Iterator Debug Level (0=disabled, 1=basic, 2=extended)"
)

# Add to global definitions
ADD_DEFINITIONS(-D_ITERATOR_DEBUG_LEVEL=${_ITERATOR_DEBUG_LEVEL})

# MSVC Runtime Library configuration
SET(FU_MSVC_RUNTIME_DEBUG
    "MultiThreadedDebugDLL"
    CACHE STRING "MSVC Runtime Library for Debug"
)
SET(FU_MSVC_RUNTIME_RELEASE
    "MultiThreadedDLL"
    CACHE STRING "MSVC Runtime Library for Release"
)

# Build configuration detection and setup
IF(CMAKE_BUILD_TYPE STREQUAL "Debug")
  SET(FU_CONF_DEBUG
      ON
      CACHE BOOL "Debug configuration flag"
  )
  SET(FU_CONF_RELEASE
      OFF
      CACHE BOOL "Release configuration flag"
  )
  SET(FU_CONF_STR
      "Debug"
      CACHE STRING "Current configuration string"
  )
ELSEIF(CMAKE_BUILD_TYPE STREQUAL "Release")
  SET(FU_CONF_DEBUG
      OFF
      CACHE BOOL "Debug configuration flag"
  )
  SET(FU_CONF_RELEASE
      ON
      CACHE BOOL "Release configuration flag"
  )
  SET(FU_CONF_STR
      "Release"
      CACHE STRING "Current configuration string"
  )
ENDIF()

# Platform detection
IF(APPLE)
  SET(FUEGO_PLATFORM
      "macos"
      CACHE STRING "Current platform"
  )
ELSEIF(WIN32)
  SET(FUEGO_PLATFORM
      "win"
      CACHE STRING "Current platform"
  )
ENDIF()

# Testing configuration
OPTION(ENABLE_FUEGO_TEST "Enable Fuego testing" OFF)

# Library type configuration
SET(FUEGO_LIB_TYPE
    "STATIC"
    CACHE STRING "Library type (STATIC/SHARED)"
)
