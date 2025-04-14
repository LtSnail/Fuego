#pragma once

#include "Input.h"
#include "singleton.hpp"

namespace Fuego
{
class InputMacOS : public Input, public singleton<InputMacOS>
{
    friend class singleton<InputMacOS>;

protected:
    virtual bool IsKeyPressedImpl(KeyCode keyCode) const override;

    virtual bool IsMouseButtonPressedImpl(MouseCode mouseCode) override;
    virtual std::pair<float, float> GetMousePositionImpl() const override;
    virtual float GetMouseXImpl() const override;
    virtual float GetMouseYImpl() const override;
    virtual glm::vec2 GetMouseDirImpl() const override;

    friend class WindowWin;
};
}  // namespace Fuego
