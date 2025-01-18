#pragma once

#include "KeyCodes.h"
#include "MouseCodes.h"
#include "glm/ext.hpp"
#include "glm/glm.hpp"

namespace Fuego
{
class FUEGO_API Input
{
public:
    enum KeyState
    {
        KEY_NONE = 0,
        KEY_PRESSED = 1,
        KEY_REPEAT = 2,
        KEY_RELEASED = 3
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

    static inline bool IsKeyPressed(KeyCode keyCode)
    {
        return m_Instance->IsKeyPressedImpl(keyCode);
    };

    static inline bool IsMouseButtonPressed(MouseCode mouseCode)
    {
        return m_Instance->IsMouseButtonPressedImpl(mouseCode);
    };

    static inline float GetMouseX()
    {
        return m_Instance->GetMouseXImpl();
    };

    static inline float GetMouseY()
    {
        return m_Instance->GetMouseYImpl();
    };
    static inline std::pair<float, float> GetMousePosition()
    {
        return m_Instance->GetMousePositionImpl();
    }
    static inline glm::vec2 GetMouseDir()
    {
        return m_Instance->GetMouseDirImpl();
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

    struct MouseInfo
    {
        MouseState state;
        MouseCode mouseCode;
    };

protected:
    virtual bool IsKeyPressedImpl(KeyCode keyCode) const = 0;

    virtual bool IsMouseButtonPressedImpl(MouseCode mouseCode) = 0;
    virtual std::pair<float, float> GetMousePositionImpl() const = 0;
    virtual float GetMouseXImpl() const = 0;
    virtual float GetMouseYImpl() const = 0;
    virtual glm::vec2 GetMouseDirImpl() const = 0;

private:
    static Input* m_Instance;
};
}  // namespace Fuego
