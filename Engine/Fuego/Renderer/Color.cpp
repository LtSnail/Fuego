#include "Color.h"

Fuego::Graphics::Color::Color(uint8_t val, uint8_t val_2, uint8_t val_3, uint8_t val_4)
{
    data = (val << 0) | (val_2 << 8) | (val_3 << 16) | (val_4 << 24);
}

// Fuego::Graphics::Color1::Color1(uint8_t val) : Color(val, 0, 0, 0) {}
//
// Fuego::Graphics::Color2::Color2(uint8_t val, uint8_t val_2) : Color(val, val_2, 0, 0) {}
//
// Fuego::Graphics::Color3::Color3(uint8_t val, uint8_t val_2, uint32_t val_3) : Color(val, val_2, val_3, 0) {}
//
// Fuego::Graphics::Color4::Color4(uint8_t val, uint8_t val_2, uint32_t val_3, uint32_t val_4)
//     : Color(val, val_2, val_3, val_4)
//{
// }
