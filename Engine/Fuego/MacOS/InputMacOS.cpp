#include "InputMacOS.h"

#include "Application.h"
#include "WindowMacOS.h"

namespace Fuego
{
bool InputMacOS::IsKeyPressedImpl(KeyCode keyCode) const
{
    UNUSED(keyCode);
    return false;
}

bool InputMacOS::IsMouseButtonPressedImpl(uint16_t mouseCode)
{
    UNUSED(mouseCode);
    return false;
}

std::pair<float, float> InputMacOS::GetMousePositionImpl() const
{
    return {0.0f, 0.0f};
}

float InputMacOS::GetMouseXImpl() const
{
    return 0.0f;
}

float InputMacOS::GetMouseYImpl() const
{
    return 0.0f;
}

glm::vec2 InputMacOS::GetMouseDirImpl() const
{
    return {0.0f, 0.0f};
}

Input& Input::platform_instance()
{
    return InputMacOS::instance();
}
}  // namespace Fuego
