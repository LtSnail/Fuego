#pragma once

#include "Input.h"
#include "singleton.hpp"

namespace Fuego
{
class InputWin final : public Input, public singleton<InputWin>
{
    friend class singleton<InputWin>;

   protected:
    virtual bool IsKeyPressedImpl(KeyCode keyCode) const override;

    virtual bool IsMouseButtonPressedImpl(MouseCode mouseCode) override;
    virtual bool IsMouseWheelScrolledImpl(std::pair<float, float>& pair) const override;
    virtual std::pair<float, float> GetMousePositionImpl() const override;
    virtual float GetMouseXImpl() const override;
    virtual float GetMouseYImpl() const override;
    virtual glm::vec2 GetMouseDirImpl() const override;

    Input::KeyInfo _lastKey;
    Input::MouseInfo _lastMouse;
};
}  // namespace Fuego
