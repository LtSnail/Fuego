#include "SwapchainOpenGL.h"

#include "OpenGL/SurfaceOpenGL.h"
#include "TextureOpenGL.h"
#include "glad/glad.h"

namespace Fuego::Renderer
{
SwapchainOpenGL::SwapchainOpenGL(const Surface& surface)
    : _surface(dynamic_cast<const SurfaceOpenGL&>(surface))
{
    const auto hdc = _surface.GetHDC();
    int pixelFormat = ChoosePixelFormat(hdc, _surface.GetPFD());
    SetPixelFormat(hdc, pixelFormat, _surface.GetPFD());
    _ctx = wglCreateContext(hdc);
    wglMakeCurrent(hdc, _ctx);

    if (!gladLoadGL())
    {
        FU_CORE_CRITICAL("[OpenGL] GLAD failed to load!");
    }

    FU_CORE_INFO("OpenGL info:");
    FU_CORE_INFO("  Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    FU_CORE_INFO("  GLSL Version: {0}", reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
    FU_CORE_INFO("  GPU Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
    FU_CORE_INFO("  Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
}

SwapchainOpenGL::~SwapchainOpenGL()
{
    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(_ctx);
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

Surface& SwapchainOpenGL::GetScreenTexture()
{
    return _surface;
}
}  // namespace Fuego::Renderer
