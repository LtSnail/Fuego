#include "SurfaceOpenGL.h"

namespace Fuego::Renderer
{
SurfaceOpenGL::SurfaceOpenGL(HWND handle)
    : _handle(nullptr)
    , _hdc(nullptr)
    , _pfd{}
{
    _handle = handle;
    _hdc = GetDC(_handle);
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

const void* SurfaceOpenGL::GetNativeHandle() const
{
    return _handle;
}

HWND SurfaceOpenGL::GetWindowsHandle() const
{
    return _handle;
}

HDC SurfaceOpenGL::GetHDC() const
{
    return _hdc;
}

const PIXELFORMATDESCRIPTOR* SurfaceOpenGL::GetPFD() const
{
    return &_pfd;
}

}  // namespace Fuego::Renderer
