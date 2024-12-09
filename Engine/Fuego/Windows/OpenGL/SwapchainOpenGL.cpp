#include "SwapchainOpenGL.h"

#include "OpenGL/SurfaceWindows.h"
#include "TextureStab.h"
#include "glad/glad.h"

namespace Fuego::Renderer

{

SwapchainOpenGL::SwapchainOpenGL(const Surface& surface)
    : _surface(dynamic_cast<const SurfaceWindows&>(surface))
{
}

void SwapchainOpenGL::Present()
{
    static PAINTSTRUCT ps;
    BeginPaint(_surface.GetWindowsHandle(), &ps);
    EndPaint(_surface.GetWindowsHandle(), &ps);
    glClearColor(0.2f, 0.3f, 0.1f, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);  // TODO: move from here
    SwapBuffers(_surface.GetHDC());
}

std::shared_ptr<Texture> SwapchainOpenGL::GetTexture()
{
    return std::make_shared<TextureStab>();
}

}  // namespace Fuego::Renderer
