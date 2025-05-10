#include "SurfaceOpenGL.h"

#include <glad/gl.h>

namespace Fuego::Graphics
{
SurfaceOpenGL::SurfaceOpenGL(const void* window)
{
    _window = (HWND)window;
    _hdc = GetDC(_window);
}

SurfaceOpenGL::~SurfaceOpenGL()
{
    Release();
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

void SurfaceOpenGL::Clear() const
{
    glClearColor(1.f, 1.f, 1.f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SurfaceOpenGL::Release()
{
    if (_hdc)
        ReleaseDC(_window, _hdc);
    _hdc = nullptr;

}

}  // namespace Fuego::Graphics
