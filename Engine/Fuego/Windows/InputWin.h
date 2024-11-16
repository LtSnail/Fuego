#pragma once

#include "Input.h"

namespace Fuego
{
class InputWin : public Input
{
   protected:
    virtual bool IsKeyPressedImpl(uint16_t keyCode) override;

    virtual bool IsMouseButtonPressedImpl(uint16_t mouseCode) override;
    virtual std::pair<float, float> GetMousePositionImpl() const override;
    virtual float GetMouseXImpl() const override;
    virtual float GetMouseYImpl() const override;

    friend class WindowWin;

   private:
    InputWin() = default;
};
}  // namespace Fuego