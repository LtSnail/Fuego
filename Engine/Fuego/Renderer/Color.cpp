#include "Color.h"

Fuego::Graphics::Color::Color(int val, int val_2, int val_3, int val_4)
{
    data[0] = static_cast<uint8_t>(val);
    data[1] = static_cast<uint8_t>(val_2);
    data[2] = static_cast<uint8_t>(val_3);
    data[3] = static_cast<uint8_t>(val_4);
}

Fuego::Graphics::Color::Color(float val, float val_2, float val_3, float val_4)
{
    data[0] = convert_float_to_byte(val);
    data[1] = convert_float_to_byte(val_2);
    data[2] = convert_float_to_byte(val_3);
    data[3] = convert_float_to_byte(val_4);
}

uint32_t Fuego::Graphics::Color::Channels(const Color& color)
{
    uint32_t count = 4;
    for (size_t i = 3; i >= 0; i--)
    {
        if (color.data[i] > 0)
            return count;
        --count;
    }
}

uint8_t Fuego::Graphics::Color::convert_float_to_byte(float val) const
{
    return static_cast<uint8_t>(val * 255.0f + 0.5f);
}
