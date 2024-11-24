#include "InputWin.h"

#include "Application.h"
#include "WindowWin.h"

namespace Fuego
{
Input* Input::m_Instance = nullptr;

bool InputWin::IsKeyPressedImpl(KeyCode keyCode)
{
    const WindowWin& window = reinterpret_cast<const WindowWin&>(Application::Get().GetWindow());
    Input::KeyState state = window.GetKeyState(keyCode);
    return state == Input::KEY_PRESSED || state == Input::KEY_REPEAT;
}
bool InputWin::IsMouseButtonPressedImpl(uint16_t mouseCode)
{
    const WindowWin& window = reinterpret_cast<const WindowWin&>(Application::Get().GetWindow());
    Input::MouseState state = window.GetMouseState(mouseCode);
    return state == Input::MOUSE_LPRESSED || state == Input::MOUSE_RPRESSED;
}
std::pair<float, float> InputWin::GetMousePositionImpl() const
{
    const WindowWin& window = reinterpret_cast<const WindowWin&>(Application::Get().GetWindow());
    float xPos, yPos;
    window.GetCursorPos(xPos, yPos);
    return {xPos, yPos};
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
}  // namespace Fuego
