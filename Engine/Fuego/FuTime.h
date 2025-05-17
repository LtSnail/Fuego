#pragma once

namespace Fuego
{
class Time
{
public:
    virtual ~Time() = default;
    static std::unique_ptr<Time> CreateTimeManager(float fixed_time);

    virtual void Tick() = 0;

    inline uint64_t Frame() const
    {
        return frame;
    }
    inline float DeltaTime() const
    {
        return delta_time;
    }
    inline float FixedTime() const
    {
        return fixed_time;
    }
    inline uint16_t FPS() const
    {
        return fps;
    }
    inline float AverageFrameTime() const
    {
        return average_frametime;
    }


protected:
    Time(float fixed_time)
        : frame(0)
        , fixed_time(fixed_time)
        , delta_time(0.f)
        , fps(0)
        , fps_time(0.f)
        , fps_frames(0)
        , average_frametime(0.f)
        , application_epoch(0) {};

    uint64_t frame;
    float fixed_time;
    float delta_time;
    uint16_t fps;
    float fps_time;
    uint16_t fps_frames;

    float average_frametime;

    uint32_t application_epoch;
};
}  // namespace Fuego