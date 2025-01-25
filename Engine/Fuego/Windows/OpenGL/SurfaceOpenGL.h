#pragma once

#include "Renderer/Surface.h"

namespace Fuego::Renderer
{
class SurfaceOpenGL final : public Surface
{
public:
    SurfaceOpenGL(const void* window);
    virtual ~SurfaceOpenGL() override;

    virtual const void* GetNativeHandle() const override;
    HDC GetHdc() const;

private:
    HWND _window;  // shouldn't be here
    HDC _hdc;      // WIN32: Handle to Device Context
};
}  // namespace Fuego::Renderer
