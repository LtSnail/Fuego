#pragma once

#include "Core.h"
#include "Metal/MetalContext.h"
#include "Window.h"

namespace Fuego
{
class WindowMacOS : public Window
{
public:
    WindowMacOS(const WindowProps& props, EventQueue& eventQueue);
    ~WindowMacOS();

    virtual void Update() override;

    inline virtual unsigned int GetWidth() const override
    {
        return m_Props.Width;
    }
    inline virtual unsigned int GetHeight() const override
    {
        return m_Props.Height;
    }

    inline virtual void SetVSync(bool enabled) override
    {
        UNUSED(enabled);
    }
    inline virtual bool IsVSync() const override
    {
        return true;
    }
    
    virtual void* GetNativeWindow() const override;

private:
    void Init(const WindowProps& props, EventQueue& eventQueue);
    void Shutdown();

    // FuegoWindow*
    void* m_Window;

    // FuegoView*
    void* m_View;

    MetalContext* _context;

    WindowProps m_Props;
};
}  // namespace Fuego
