
# --- Global vars --- #
# string:                   ->    FUEGO_PLATFORM ("macos"/"win")
# var defenition            ->    FUEGO_PLATFORM_MACOS/FUEGO_PLATFORM_WIN
# var defenition            ->    ENABLE_FUEGO_TEST
# cmake var integer         ->    FU_CPP_LANG_VER
# string                    ->    FU_MSVC_RUNTIME_DEBUG("MultiThreadedDebugDLL")/FU_MSVC_RUNTIME_RELEASE("MultiThreadedDLL")
# var defenition            ->    FU_CONF_RELEASE
# var defenition            ->    FU_CONF_DEBUG
# string                    ->    FU_CONF_STR("Debug"/"Release")

set(FU_CPP_LANG_VER "20" CACHE STRING  "C++ Language standard version")
set(FU_MSVC_RUNTIME_DEBUG "MultiThreadedDebugDLL" CACHE STRING "MSCV Runtime Library")
set(FU_MSVC_RUNTIME_RELEASE "MultiThreadedDLL" CACHE STRING "MSCV Runtime Library")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(FU_CONF_RELEASE ON CACHE BOOL "Current configuration")
    set(FU_CONF_STR "Debug" CACHE STRING "Current configuration")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(FU_CONF_DEBUG ON CACHE BOOL "Current configuration")
    set(FU_CONF_STR "Release" CACHE STRING "Current configuration")
endif()

