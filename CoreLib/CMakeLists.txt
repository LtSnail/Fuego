# === CoreLib - Independent Utility Library ===

# === Source Files ===
SET(CORELIB_HEADERS CoreLibConcepts.h Uptr.h Sptr.h singleton.hpp DefaultAllocator.h)

SET(CORELIB_SOURCES CoreLib.cpp)

# === Library Target ===
ADD_LIBRARY(CoreLib STATIC ${CORELIB_SOURCES} ${CORELIB_HEADERS})

# === C++ Standard ===
TARGET_COMPILE_FEATURES(CoreLib PUBLIC cxx_std_${FU_CPP_LANG_VER})

# === Include Directories ===
TARGET_INCLUDE_DIRECTORIES(CoreLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# === Compile Options ===
IF(MSVC)
  TARGET_COMPILE_OPTIONS(CoreLib PRIVATE /W4 /WX)
ELSE()
  TARGET_COMPILE_OPTIONS(CoreLib PRIVATE -Wall -Wextra -Werror)
ENDIF()

# === Compile Definitions ===
TARGET_COMPILE_DEFINITIONS(
  CoreLib PRIVATE CORELIB_VERSION_MAJOR=${CMAKE_PROJECT_VERSION_MAJOR}
                  CORELIB_VERSION_MINOR=${CMAKE_PROJECT_VERSION_MINOR} CORELIB_BUILD_LIB
)

# === Testing Support ===
IF(ENABLE_FUEGO_TEST)
  TARGET_INCLUDE_DIRECTORIES(CoreLib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../External/googletest/googletest/include)
  TARGET_LINK_LIBRARIES(CoreLib PRIVATE GTest::gtest)
ENDIF()

# === IDE Organization ===
SET_TARGET_PROPERTIES(
  CoreLib
  PROPERTIES FOLDER "Core" DEBUG_POSTFIX "_d"
)
