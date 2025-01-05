#pragma once

#include "Renderer/Surface.h"

namespace Fuego::Renderer
{
class SurfaceOpenGL : public Surface
{
public:
    SurfaceOpenGL(const void* window);
    ~SurfaceOpenGL();

    virtual const void* GetNativeHandle() const override;
    HDC GetHdc() const;

private:
    HWND _window;  // shouldn't be here
    HDC _hdc;      // WIN32: Handle to Device Context
};
}  // namespace Fuego::Renderer
