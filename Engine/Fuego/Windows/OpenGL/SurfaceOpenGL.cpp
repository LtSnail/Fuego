#include "SurfaceOpenGL.h"

namespace Fuego::Renderer
{
SurfaceOpenGL::SurfaceOpenGL(const Fuego::Window* window)
    : _window(reinterpret_cast<const WindowWin*>(window))
{
}

SurfaceOpenGL::~SurfaceOpenGL()
{
}

Window& SurfaceOpenGL::GetBindedWindow() const
{
    return (Window&)*_window;
}

}  // namespace Fuego::Renderer
