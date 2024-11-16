#pragma once

#include "KeyCodes.h"
#include "MouseCodes.h"
#include "fupch.h"

namespace Fuego
{
class FUEGO_API Input
{
   public:
    static inline bool IsKeyPressed(KeyCode keyCode)
    {
        return m_Instance->IsKeyPressedImpl(keyCode);
    };

    static inline bool IsMouseButtonPressed(MouseCode mouseCode)
    {
        return m_Instance->IsMouseButtonPressedImpl(mouseCode);
    };

    static inline float GetMouseX() { return m_Instance->GetMouseXImpl(); };

    static inline float GetMouseY() { return m_Instance->GetMouseYImpl(); };
    static inline std::pair<float, float> GetMousePosition()
    {
        return m_Instance->GetMousePositionImpl();
    }
    static inline bool Init(Input* input)
    {
        if (!m_Instance && input != nullptr)
        {
            m_Instance = input;
            return true;
        }
        return false;
    };

    enum KeyState
    {
        KEY_NONE = 0,
        KEY_PRESSED = 1,
        KEY_REPEAT = 2,
    };

    struct KeyInfo
    {
        KeyState state;
        KeyCode keyCode;
    };

    enum MouseState
    {
        MOUSE_NONE,
        MOUSE_LPRESSED,
        MOUSE_RPRESSED,
        MOUSE_MPRESSED,
        MOUSE_SCROLL
    };

    struct MouseInfo
    {
        MouseState state;
        MouseCode mouseCode;
    };

    struct CursorPos
    {
        float x;
        float y;
    };

   protected:
    virtual bool IsKeyPressedImpl(KeyCode keyCode) = 0;

    virtual bool IsMouseButtonPressedImpl(MouseCode mouseCode) = 0;
    virtual std::pair<float, float> GetMousePositionImpl() const = 0;
    virtual float GetMouseXImpl() const = 0;
    virtual float GetMouseYImpl() const = 0;

   private:
    static Input* m_Instance;
};
}  // namespace Fuego