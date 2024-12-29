#include "SwapchainOpenGL.h"

#include "OpenGL/SurfaceOpenGL.h"
#include "TextureOpenGL.h"
#include "glad/glad.h"

namespace Fuego::Renderer
{
SwapchainOpenGL::SwapchainOpenGL(const Surface& surface)
    : _surface(dynamic_cast<const SurfaceOpenGL&>(surface))
{
    const auto hdc = _surface.GetHdc();
    int pixelFormat = ChoosePixelFormat(hdc, _surface.GetPfd());
    SetPixelFormat(hdc, pixelFormat, _surface.GetPfd());
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
    // TODO: if debuf then enable OpenGL debug callback:
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(SwapchainOpenGL::OpenGLDebugCallbackFunc, nullptr);
    glEnable(GL_DEPTH_TEST);
}

SwapchainOpenGL::~SwapchainOpenGL()
{
    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(_ctx);
}

void SwapchainOpenGL::Present()
{
    static PAINTSTRUCT ps;
    BeginPaint((HWND)_surface.GetNativeHandle(), &ps);
    EndPaint((HWND)_surface.GetNativeHandle(), &ps);
    SwapBuffers(_surface.GetHdc());
}

Surface& SwapchainOpenGL::GetScreenTexture()
{
    return _surface;
}

void APIENTRY SwapchainOpenGL::OpenGLDebugCallbackFunc(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message,
                                                       const void* userParam)
{
    GLenum error = glGetError();
    if (error == GL_NO_ERROR)
    {
        FU_CORE_INFO("[OpenGL] \nMessage: {0}\nSource: {1}\nType: {2}\nID: {3}\nSeverity: {4}",
            (const char*)message, source, type, id, severity);
    }
    else
    {
        FU_CORE_ERROR("[OpenGL] \nMessage: {0}\nSource: {1}\nType: {2}\nID: {3}\nSeverity: {4}",
            (const char*)message, source, type, id, severity);
    }
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
}  // namespace Fuego::Renderer
