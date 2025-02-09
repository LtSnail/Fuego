#include "SwapchainOpenGL.h"

#include "OpenGL/SurfaceOpenGL.h"
// clang-format off
#include "glad/wgl.h"
#include "glad/gl.h"
// clang-format on

namespace Fuego::Renderer
{
SwapchainOpenGL::SwapchainOpenGL(const Surface& surface)
    : _surface(dynamic_cast<const SurfaceOpenGL&>(surface))
{
    auto rect = surface.GetRect();
    glViewport(rect.x, rect.y, rect.width, rect.height);
}

SwapchainOpenGL::~SwapchainOpenGL()
{
}

void SwapchainOpenGL::Present()
{
    SwapBuffers(_surface.GetHdc());
}

Surface& SwapchainOpenGL::GetScreenTexture()
{
    return _surface;
}

void SwapchainOpenGL::ShowWireFrame(bool show)
{
    if (show)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

void SwapchainOpenGL::ValidateWindow()
{
    static PAINTSTRUCT ps;
    BeginPaint((HWND)_surface.GetNativeHandle(), &ps);
    EndPaint((HWND)_surface.GetNativeHandle(), &ps);
}
}  // namespace Fuego::Renderer
