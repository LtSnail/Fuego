#pragma once

#include "fupch.h"

namespace Fuego
{
class FUEGO_API Input
{
   public:
    static inline bool IsKeyPressed(uint16_t keyCode)
    {
        return m_Instance->IsKeyPressedImpl(keyCode);
    };

    static inline bool IsMouseButtonPressed(uint16_t mouseCode)
    {
        return m_Instance->IsMouseButtonPressedImpl(mouseCode);
    };

    static inline float GetMouseX() { return m_Instance->GetMouseXImpl(); };

    static inline float GetMouseY() { return m_Instance->GetMouseYImpl(); };
    static inline std::pair<float, float> GetMousePosition()
    {
        return m_Instance->GetMousePositionImpl();
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

   protected:
    virtual bool IsKeyPressedImpl(uint16_t keyCode) = 0;

    virtual bool IsMouseButtonPressedImpl(uint16_t mouseCode) = 0;
    virtual std::pair<float, float> GetMousePositionImpl() const = 0;
    virtual float GetMouseXImpl() const = 0;
    virtual float GetMouseYImpl() const = 0;

   private:
    static Input* m_Instance;
};
}  // namespace Fuego