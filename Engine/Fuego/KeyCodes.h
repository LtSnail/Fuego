#pragma once
#include <cstdint>

namespace Fuego
{
using KeyCode = uint16_t;
namespace Key
{
enum : KeyCode
{
    // clang-format off
    Space           = 0,
    Apostrophe      = 1,
    Comma           = 2,
    Minus           = 3,
    Period          = 4,
    Slash           = 5,
                    
    D0              = 10,
    D1              = 11,
    D2              = 12,
    D3              = 13,
    D4              = 14,
    D5              = 15,
    D6              = 16,
    D7              = 17,
    D8              = 18,
    D9              = 19,
                    
    Semicolon       = 20,
    Equal           = 21,

    A               = 30,
    B               = 31,
    C               = 32,
    D               = 33,
    E               = 34,
    F               = 35,
    G               = 36,
    H               = 37,
    I               = 38,
    J               = 39,
    K               = 40,
    L               = 41,
    M               = 42,
    N               = 43,
    O               = 44,
    P               = 45,
    Q               = 46,
    R               = 47,
    S               = 48,
    T               = 49,
    U               = 50,
    V               = 51,
    W               = 52,
    X               = 53,
    Y               = 54,
    Z               = 55,

    LeftBracket     = 60,  /* [ */
    Backslash       = 61,    /* \ */
    RightBracket    = 62, /* ] */
    GraveAccent     = 63,  /* ` */

    World1          = 70, /* non-US #1 */
    World2          = 71, /* non-US #2 */

    /* Function keys */
    Escape          = 80,
    Enter           = 81,
    Tab             = 82,
    Backspace       = 83,
    Insert          = 84,
    Delete          = 85,
    Right           = 86,
    Left            = 87,
    Down            = 88,
    Up              = 89,
    PageUp          = 90,
    PageDown        = 91,
    Home            = 92,
    End             = 93,
    CapsLock        = 94,
    ScrollLock      = 95,
    NumLock         = 96,
    PrintScreen     = 97,
    Pause           = 98,

    F1              = 100,
    F2              = 101,
    F3              = 102,
    F4              = 103,
    F5              = 104,
    F6              = 105,
    F7              = 106,
    F8              = 107,
    F9              = 108,
    F10             = 109,
    F11             = 110,
    F12             = 111,
    F13             = 112,
    F14             = 113,
    F15             = 114,
    F16             = 115,
    F17             = 116,
    F18             = 117,
    F19             = 118,
    F20             = 119,
    F21             = 120,
    F22             = 121,
    F23             = 122,
    F24             = 123,
    F25             = 124,

    /* Keypad */
    KP0             = 140,
    KP1             = 141,
    KP2             = 142,
    KP3             = 143,
    KP4             = 144,
    KP5             = 145,
    KP6             = 146,
    KP7             = 147,
    KP8             = 148,
    KP9             = 149,
    KPDecimal       = 160,
    KPDivide        = 161,
    KPMultiply      = 162,
    KPSubtract      = 163,
    KPAdd           = 164,
    KPEnter         = 165,
    KPEqual         = 166,

    LeftShift       = 180,
    LeftControl     = 181,
    LeftAlt         = 182,
    LeftSuper       = 183,
    RightShift      = 184,
    RightControl    = 185,
    RightAlt        = 186,
    RightSuper      = 187,
    Menu            = 188,
    
    Unknown
    // clang-format on
};

}
}  // namespace Fuego
