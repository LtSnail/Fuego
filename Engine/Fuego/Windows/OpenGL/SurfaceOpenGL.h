#pragma once

#include "Renderer/Surface.h"
#include "WindowWin.h"

namespace Fuego::Renderer
{
class SurfaceOpenGL : public Surface
{
public:
    SurfaceOpenGL(const Fuego::Window* window);
    ~SurfaceOpenGL();

    virtual Window& GetBindedWindow() const override;
    inline const HDC GetDescriptor() const
    {
        return _window->GetDescriptor();
    }
    inline const HWND GetHandle() const
    {
        return _window->GetHandle();
    }

private:
    const WindowWin* _window;
};
}  // namespace Fuego::Renderer
