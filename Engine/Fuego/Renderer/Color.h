#pragma once

namespace Fuego::Graphics
{

class Color
{
public:
    Color(int val, int val_2 = 0, int val_3 = 0, int val_4 = 0);
    Color(float val, float val_2 = 0.f, float val_3 = 0.f, float val_4 = 0.f);
    ~Color() = default;

    static uint32_t Channels(const Color& color);
    uint32_t Data() const
    {
        return static_cast<uint32_t>(data[0]) | (static_cast<uint32_t>(data[1]) << 8) | (static_cast<uint32_t>(data[2]) << 16) |
               (static_cast<uint32_t>(data[3]) << 24);
    }

private:
    std::array<uint8_t, 4> data;

    uint8_t convert_float_to_byte(float val) const;
};

}  // namespace Fuego::Graphics
