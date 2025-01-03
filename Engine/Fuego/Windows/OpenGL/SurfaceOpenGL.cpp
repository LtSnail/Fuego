#include "SurfaceOpenGL.h"

namespace Fuego::Renderer
{
SurfaceOpenGL::SurfaceOpenGL(const void* window)
{
    _window = (HWND)window;
    _hdc = GetDC(_window);
}

SurfaceOpenGL::~SurfaceOpenGL()
{
    ReleaseDC(_window, _hdc);
}

const void* SurfaceOpenGL::GetNativeHandle() const
{
    return _window;
}
HDC SurfaceOpenGL::GetHdc() const
{
    return _hdc;
}

}  // namespace Fuego::Renderer
