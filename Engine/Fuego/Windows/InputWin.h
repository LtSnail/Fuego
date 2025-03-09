#pragma once

#include "Input.h"

namespace Fuego
{
class InputWin final : public Input
{
protected:
    virtual bool IsKeyPressedImpl(KeyCode keyCode) const override;

    virtual bool IsMouseButtonPressedImpl(MouseCode mouseCode) override;
    virtual std::pair<float, float> GetMousePositionImpl() const override;
    virtual float GetMouseXImpl() const override;
    virtual float GetMouseYImpl() const override;
    virtual glm::vec2 GetMouseDirImpl() const override;

    Input::KeyInfo _lastKey;
    Input::MouseInfo _lastMouse;
    friend class WindowWin;

private:
    InputWin() = default;
};
}  // namespace Fuego
