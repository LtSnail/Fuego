#pragma once

#include "Input.h"

namespace Fuego
{
class InputMacOS : public Input
{
protected:
    virtual bool IsKeyPressedImpl(KeyCode keyCode) override;

    virtual bool IsMouseButtonPressedImpl(MouseCode mouseCode) override;
    virtual std::pair<float, float> GetMousePositionImpl() const override;
    virtual float GetMouseXImpl() const override;
    virtual float GetMouseYImpl() const override;

    friend class WindowWin;

private:
    InputMacOS() = default;
};
}  // namespace Fuego
