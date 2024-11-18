#pragma once

#include <Carbon/Carbon.h>

#include "KeyCodes.h"

static inline Fuego::KeyCode GetKeyCode(int key)
{
#define KEY(x) Fuego::Key::x

    switch (key)
    {
        case kVK_Space:
            return KEY(Space);
        case kVK_ANSI_A:
            return KEY(A);
        case kVK_ANSI_S:
            return KEY(S);
        case kVK_ANSI_D:
            return KEY(D);
        case kVK_ANSI_F:
            return KEY(F);
        case kVK_ANSI_H:
            return KEY(H);
        case kVK_ANSI_G:
            return KEY(G);
        case kVK_ANSI_Z:
            return KEY(Z);
        case kVK_ANSI_X:
            return KEY(X);
        case kVK_ANSI_C:
            return KEY(C);
        case kVK_ANSI_V:
            return KEY(V);
        case kVK_ANSI_B:
            return KEY(B);
        case kVK_ANSI_Q:
            return KEY(Q);
        case kVK_ANSI_W:
            return KEY(W);
        case kVK_ANSI_E:
            return KEY(E);
        case kVK_ANSI_R:
            return KEY(R);
        case kVK_ANSI_Y:
            return KEY(Y);
        case kVK_ANSI_T:
            return KEY(T);
        case kVK_ANSI_1:
            return KEY(D1);
        case kVK_ANSI_2:
            return KEY(D2);
        case kVK_ANSI_3:
            return KEY(D3);
        case kVK_ANSI_4:
            return KEY(D4);
        case kVK_ANSI_5:
            return KEY(D5);
        case kVK_ANSI_6:
            return KEY(D6);
        case kVK_ANSI_7:
            return KEY(D7);
        case kVK_ANSI_8:
            return KEY(D8);
        case kVK_ANSI_9:
            return KEY(D9);
        case kVK_ANSI_0:
            return KEY(D0);
        case kVK_ANSI_Equal:
            return KEY(Equal);
        case kVK_ANSI_Minus:
            return KEY(Minus);
        case kVK_ANSI_RightBracket:
            return KEY(RightBracket);
        case kVK_ANSI_LeftBracket:
            return KEY(LeftBracket);
        case kVK_ANSI_Quote:
            return KEY(Apostrophe);
        case kVK_ANSI_Semicolon:
            return KEY(Semicolon);
        case kVK_ANSI_Backslash:
            return KEY(Backslash);
        case kVK_ANSI_Comma:
            return KEY(Comma);
        case kVK_ANSI_Slash:
            return KEY(Slash);
        case kVK_ANSI_Period:
            return KEY(Period);
        case kVK_ANSI_Grave:
            return KEY(GraveAccent);
        case kVK_Return:
            return KEY(Enter);
        case kVK_Tab:
            return KEY(Tab);
        case kVK_Delete:
            return KEY(Delete);
        case kVK_Escape:
            return KEY(Escape);
        case kVK_Command:
            return KEY(LeftSuper);
        case kVK_Shift:
            return KEY(LeftShift);
        case kVK_CapsLock:
            return KEY(CapsLock);
        case kVK_Option:
            return KEY(LeftAlt);
        case kVK_Control:
            return KEY(LeftControl);
        default:
            FU_CORE_ASSERT(false, "Unknown key code: {}", key);
            return KEY(Unknown);
    }

#undef KEY
}
