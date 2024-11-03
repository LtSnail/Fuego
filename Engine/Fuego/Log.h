#pragma once

#include "Core.h"

#include "spdlog/spdlog.h"


namespace Fuego
{
    class  Log
    {
    public:
        FUEGO_API static void Init();

        FUEGO_API inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        FUEGO_API inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };
}

// Core Log macros.
#define FU_CORE_TRACE(...)   ::Fuego::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define FU_CORE_INFO(...)    ::Fuego::Log::GetCoreLogger()->info(__VA_ARGS__)
#define FU_CORE_WARN(...)    ::Fuego::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define FU_CORE_ERROR(...)   ::Fuego::Log::GetCoreLogger()->error(__VA_ARGS__)
#define FU_CORE_FATAL(...)   ::Fuego::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client Log macros.
#define FU_TRACE(...)        ::Fuego::Log::GetClientLogger()->trace(__VA_ARGS__)
#define FU_INFO(...)         ::Fuego::Log::GetClientLogger()->info(__VA_ARGS__)
#define FU_WARN(...)         ::Fuego::Log::GetClientLogger()->warn(__VA_ARGS__)
#define FU_ERROR(...)        ::Fuego::Log::GetClientLogger()->error(__VA_ARGS__)
#define FU_FATAL(...)        ::Fuego::Log::GetClientLogger()->fatal(__VA_ARGS__)
