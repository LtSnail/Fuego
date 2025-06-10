#include "TimeMacOS.h"


std::unique_ptr<Fuego::Time> Fuego::Time::CreateTimeManager(float fixed_time)
{
    return std::make_unique<Fuego::TimeMacOS>(fixed_time);
}

Fuego::TimeMacOS::TimeMacOS(float fixed_time)
    : Time(fixed_time)
    , timer(std::chrono::steady_clock::now())
{
}

void Fuego::TimeMacOS::Tick()
{
}

void Fuego::TimeMacOS::calc_delta_time()
{
}
