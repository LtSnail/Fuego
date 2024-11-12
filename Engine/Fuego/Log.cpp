#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace Fuego
{
class Log::LogImpl
{
   private:
    friend class Log;
    static std::shared_ptr<spdlog::logger> s_CoreLogger;
    static std::shared_ptr<spdlog::logger> s_ClientLogger;
};

std::shared_ptr<spdlog::logger> Log::LogImpl::s_CoreLogger;
std::shared_ptr<spdlog::logger> Log::LogImpl::s_ClientLogger;

void Log::Init()
{
    spdlog::set_pattern("%^[%T] %n: %v%$");

    LogImpl::s_CoreLogger = spdlog::stdout_color_mt("Fuego");
    LogImpl::s_CoreLogger->set_level(spdlog::level::trace);

    LogImpl::s_ClientLogger = spdlog::stdout_color_mt("APP");
    LogImpl::s_ClientLogger->set_level(spdlog::level::trace);

    FU_CORE_TRACE("[LOG] has been initialized");
}
std::shared_ptr<spdlog::logger>& Log::GetCoreLogger()
{
    return LogImpl::s_CoreLogger;
}
std::shared_ptr<spdlog::logger>& Log::GetClientLogger()
{
    return LogImpl::s_ClientLogger;
}
}  // namespace Fuego
