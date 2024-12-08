#pragma once

#include "Renderer/Surface.h"

namespace Fuego::Renderer
{
class SurfaceWindows : public Surface
{
public:
    SurfaceWindows(void* handle);
    virtual ~SurfaceWindows() override = default;

    virtual const void* GetNativeHandle() const override;
    const HWND GetWindowsHandle() const;
    const HDC GetHDC() const;
    const PIXELFORMATDESCRIPTOR* GetPFD() const;

protected:
private:
    HWND _handle;                // WIN32: Window descriptor
    HDC _hdc;                    // WIN32: Handle to Device Context
    PIXELFORMATDESCRIPTOR _pfd;  // WIN32: Pixel format descriptor
};
}  // namespace Fuego::Renderer
