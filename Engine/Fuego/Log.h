#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"

namespace Fuego
{
class FUEGO_API Log
{
private:
    class LogImpl;

public:
    static void Init();
    static std::shared_ptr<spdlog::logger>& GetCoreLogger();
    static std::shared_ptr<spdlog::logger>& GetClientLogger();
};
}  // namespace Fuego

// clang-format off
#if defined(FUEGO_BUILD_LIB)
#define FU_CORE_TRACE(...) ::Fuego::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define FU_CORE_INFO(...) ::Fuego::Log::GetCoreLogger()->info(__VA_ARGS__)
#define FU_CORE_WARN(...) ::Fuego::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define FU_CORE_ERROR(...) ::Fuego::Log::GetCoreLogger()->error(__VA_ARGS__)
#define FU_CORE_CRITICAL(...) ::Fuego::Log::GetCoreLogger()->critical(__VA_ARGS__)
#endif

#if !defined(FUEGO_BUILD_LIB)
#define FU_TRACE(...) ::Fuego::Log::GetClientLogger()->trace(__VA_ARGS__)
#define FU_INFO(...) ::Fuego::Log::GetClientLogger()->info(__VA_ARGS__)
#define FU_WARN(...) ::Fuego::Log::GetClientLogger()->warn(__VA_ARGS__)
#define FU_ERROR(...) ::Fuego::Log::GetClientLogger()->error(__VA_ARGS__)
#define FU_CRITICAL(...) ::Fuego::Log::GetClientLogger()->critical(__VA_ARGS__)
#endif
// clang-format on
