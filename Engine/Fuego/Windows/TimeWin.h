#pragma once

#include <chrono>

#include "FuTime.h"


namespace Fuego
{
class TimeWin : public Time
{
public:
    TimeWin(float fixed_time);
    virtual ~TimeWin() override = default;

    virtual void Tick() override;

private:
    std::chrono::time_point<std::chrono::steady_clock> timer;
    void calc_delta_time();
};
}  // namespace Fuego