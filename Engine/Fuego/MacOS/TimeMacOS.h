#pragma once

#include <chrono>

#include "FuTime.h"

namespace Fuego
{
class TimeMacOS : public Time
{
public:
    TimeMacOS(float fixed_time);
    virtual ~TimeMacOS() override = default;

    virtual void Tick() override;

private:
    std::chrono::time_point<std::chrono::steady_clock> timer;
    void calc_delta_time();
};
}  // namespace Fuego
