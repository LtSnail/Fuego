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

Surface::Rect SurfaceOpenGL::GetRect() const
{
    RECT rect;
    if (GetClientRect(_window, &rect))
    {
        return {rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top};
    }

    return {0, 0, 0, 0};
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
