#pragma once

#include "Renderer/Surface.h"

namespace Fuego::Renderer
{
class SurfaceOpenGL : public Surface
{
public:
    SurfaceOpenGL(HWND handle);
    virtual ~SurfaceOpenGL() = default;

    virtual const void* GetNativeHandle() const override;
    HWND GetWindowsHandle() const;
    HDC GetHDC() const;
    const PIXELFORMATDESCRIPTOR* GetPFD() const;

private:
    HWND _handle;                // WIN32: Window descriptor
    HDC _hdc;                    // WIN32: Handle to Device Context
    PIXELFORMATDESCRIPTOR _pfd;  // WIN32: Pixel format descriptor
};
}  // namespace Fuego::Renderer
