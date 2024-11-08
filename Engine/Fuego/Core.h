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

#if defined(FUEGO_ENABLE_ASSERTS)
#if defined(FUEGO_PLATFORM_WIN)
#define FU_ASSERT(x, ...)                                  \
    {                                                      \
        if (!(x))                                          \
        {                                                  \
            FU_ERROR("Assertion Failed {0}", __VA_ARGS__); \
            __debugbreak();                                \
        }                                                  \
    }
#define FU_CORE_ASSERT(x, ...)                                  \
    {                                                           \
        if (!(x))                                               \
        {                                                       \
            FU_CORE_ERROR("Assertion Failed {0}", __VA_ARGS__); \
            __debugbreak();                                     \
        }                                                       \
    }
#elif defined(FUEGO_PLATFORM_MACOS)
#define FU_ASSERT(x, ...)                                  \
    {                                                      \
        if (!(x))                                          \
        {                                                  \
            FU_ERROR("Assertion Failed {0}", __VA_ARGS__); \
            __builtin_debugtrap();                         \
        }                                                  \
    }
#define FU_CORE_ASSERT(x, ...)                                  \
    {                                                           \
        if (!(x))                                               \
        {                                                       \
            FU_CORE_ERROR("Assertion Failed {0}", __VA_ARGS__); \
            __builtin_debugtrap();                              \
        }                                                       \
    }
#endif
#else
#define FU_ASSERT(x, ...)
#define FU_CORE_ASSERT(x, ...)
#endif

#if defined(FUEGO_BUILD_LIB)
#define FUEGO_API EXPORT
#else
#define FUEGO_API IMPORT
#endif

#define FUEGO_NON_COPYABLE_NON_MOVABLE(ClassName)    \
    ClassName(const ClassName&) = delete;            \
    ClassName& operator=(const ClassName&) = delete; \
    ClassName(ClassName&&) = delete;                 \
    ClassName& operator=(ClassName&&) = delete;

#define BIT(x) (1 << x)

#define UNUSED(x) (void)(x)
