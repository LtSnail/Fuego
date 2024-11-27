#pragma once

#if defined(FUEGO_PLATFORM_WIN)
#define EXPORT __declspec(dllexport)
#define IMPORT __declspec(dllimport)
#elif defined(FUEGO_PLATFORM_MACOS)
#define EXPORT __attribute__((visibility("default")))
#define IMPORT __attribute__((visibility("default")))
#else
#error Unsupported platform!
#endif

// clang-format off
#if defined(FUEGO_DYNAMIC_LIB)
    #if defined(FUEGO_BUILD_LIB)
        #define FUEGO_API EXPORT
    #else
        #define FUEGO_API IMPORT
    #endif
#else
    #define FUEGO_API
#endif
// clang-format on

#define FUEGO_NON_COPYABLE_NON_MOVABLE(ClassName)    \
    ClassName(const ClassName&) = delete;            \
    ClassName& operator=(const ClassName&) = delete; \
    ClassName(ClassName&&) = delete;                 \
    ClassName& operator=(ClassName&&) = delete;

#define FUEGO_NON_COPYABLE(ClassName)     \
    ClassName(const ClassName&) = delete; \
    ClassName& operator=(const ClassName&) = delete;

// TODO: Rewrite to use a custom unique pointer
#define FUEGO_INTERFACE(ClassName) \
private:                           \
    class ClassName##Impl;         \
    ClassName##Impl* d;

#define BIT(x) (1 << x)

#define UNUSED(x) (void)(x)
