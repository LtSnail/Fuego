#include "SwapchainOpenGL.h"

#include "OpenGL/SurfaceOpenGL.h"
#include "TextureOpenGL.h"
// clang-format off
#include "glad/wgl.h"
#include "glad/gl.h"
// clang-format on

namespace Fuego::Renderer
{
SwapchainOpenGL::SwapchainOpenGL(const Surface& surface)
    : _surface(dynamic_cast<const SurfaceOpenGL&>(surface))
{
}

SwapchainOpenGL::~SwapchainOpenGL()
{
}

void SwapchainOpenGL::Present()
{
    SwapBuffers(_surface.GetDescriptor());
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
    BeginPaint(_surface.GetHandle(), &ps);
    EndPaint(_surface.GetHandle(), &ps);
}
}  // namespace Fuego::Renderer
