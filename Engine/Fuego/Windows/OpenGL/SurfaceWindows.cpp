#include "SurfaceWindows.h"

namespace Fuego::Renderer
{

std::unique_ptr<Surface> Surface::CreateSurface(void* handle)
{
    return std::make_unique<SurfaceWindows>(handle);
}

SurfaceWindows::SurfaceWindows(void* handle)
    : _handle(nullptr)
    , _hdc(nullptr)
    , _pfd{}
{
    _handle = handle;
    _hdc = GetDC(static_cast<HWND>(handle));
    _pfd = {sizeof(PIXELFORMATDESCRIPTOR),
              1,
              PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
              PFD_TYPE_RGBA,
              32,
              0,
              0,
              0,
              0,
              0,
              0,
              0,
              0,
              0,
              0,
              0,
              0,
              0,
              24,
              8,
              0,
              PFD_MAIN_PLANE,
              0,
              0,
              0,
              0};
}

const void* SurfaceWindows::GetNativeHandle() const
{
    return static_cast<const void*>(_handle);
}

const HWND* SurfaceWindows::GetWindowsHandle() const
{
    const HWND* ptr = reinterpret_cast<const HWND*>(_handle);
    return ptr;
}

const HDC* SurfaceWindows::GetHDC()
{
    return &_hdc;
}

const PIXELFORMATDESCRIPTOR* SurfaceWindows::GetPFD() const
{
    return &_pfd;
}

}