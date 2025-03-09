#pragma once

#include "Input.h"
#include "singleton.hpp"

namespace Fuego
{
class InputMacOS : public Input, public singleton<InputWin>
{
    friend class singleton<InputWin>;

protected:
    virtual bool IsKeyPressedImpl(KeyCode keyCode) const override;

    virtual bool IsMouseButtonPressedImpl(MouseCode mouseCode) override;
    virtual std::pair<float, float> GetMousePositionImpl() const override;
    virtual float GetMouseXImpl() const override;
    virtual float GetMouseYImpl() const override;

    friend class WindowWin;
};
}  // namespace Fuego
