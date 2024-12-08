#include "SwapchainOpenGL.h"

#include "OpenGL/SurfaceWindows.h"
#include "TextureStab.h"
#include "glad/glad.h"

namespace Fuego::Renderer

{

SwapchainOpenGL::SwapchainOpenGL(const Surface& surface)
{
    const SurfaceWindows& surfWin = static_cast<const SurfaceWindows&>(surface);
    static PAINTSTRUCT ps;
    BeginPaint(surfWin.GetWindowsHandle(), &ps);
    EndPaint(surfWin.GetWindowsHandle(), &ps);
    glClearColor(0.2f, 0.3f, 0.1f, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    SwapBuffers(surfWin.GetHDC());
}

std::unique_ptr<Swapchain> Swapchain::CreateSwapChain(const Surface& surface)
{
    return std::make_unique<SwapchainOpenGL>(surface);
}

std::shared_ptr<Texture> SwapchainOpenGL::GetTexture()
{
    return std::make_shared<TextureStab>();
}

}  // namespace Fuego::Renderer
