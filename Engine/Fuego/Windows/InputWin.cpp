#include "InputWin.h"

#include "Application.h"
#include "WindowWin.h"

namespace Fuego
{
template <>
InputWin& singleton<InputWin>::instance()
{
    static InputWin inst;
    return inst;
}

bool InputWin::IsKeyPressedImpl(KeyCode keyCode) const
{
    const WindowWin& window = static_cast<const WindowWin&>(Application::instance().GetWindow());
    Input::KeyState state = window.GetKeyState(keyCode);
    return state == Input::KEY_PRESSED || state == Input::KEY_REPEAT;
}

bool InputWin::IsMouseButtonPressedImpl(uint16_t mouseCode)
{
    const WindowWin& window = static_cast<const WindowWin&>(Application::instance().GetWindow());
    Input::MouseState state = window.GetMouseState(mouseCode);
    return state == Input::MOUSE_LPRESSED || state == Input::MOUSE_RPRESSED;
}

std::pair<float, float> InputWin::GetMousePositionImpl() const
{
    const WindowWin& window = static_cast<const WindowWin&>(Application::instance().GetWindow());
    float xPos, yPos;
    window.GetMousePos(xPos, yPos);
    return {xPos, yPos};
}

bool InputWin::IsMouseWheelScrolledImpl(std::pair<float, float>& pair) const
{
    const WindowWin& window = static_cast<const WindowWin&>(Application::instance().GetWindow());
    pair = window.GetMouseWheelScrollData();
    if (pair.first != 0.f || pair.second != 0.f)
        return true;
    return false;
}

float InputWin::GetMouseXImpl() const
{
    auto [x, y] = GetMousePositionImpl();
    return x;
}

float InputWin::GetMouseYImpl() const
{
    auto [x, y] = GetMousePositionImpl();
    return y;
}

glm::vec2 InputWin::GetMouseDirImpl() const
{
    const WindowWin& window = static_cast<const WindowWin&>(Application::instance().GetWindow());
    return window.GetMouseDir();
}

Input& Input::platform_instance()
{
    return InputWin::instance();
}
}  // namespace Fuego
