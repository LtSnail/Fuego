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
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(SwapchainOpenGL::OpenGLDebugCallbackFunc, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

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
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
        return;
    FU_CORE_ERROR("---- OpenGL Debug Message ----");
    switch (source)
    {
    case GL_DEBUG_SOURCE_API:
        FU_CORE_ERROR("Source: API");
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        FU_CORE_ERROR("Source: Window System");
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        FU_CORE_ERROR("Source: Shader Compiler");
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        FU_CORE_ERROR("Source: Third Party");
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        FU_CORE_ERROR("Source: Application");
        break;
    case GL_DEBUG_SOURCE_OTHER:
        FU_CORE_ERROR("Source: Other");
        break;
    }

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:
        FU_CORE_ERROR("Type: Error");
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        FU_CORE_ERROR("Type: Deprecated Behaviour");
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        FU_CORE_ERROR("Type: Undefined Behaviour");
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        FU_CORE_ERROR("Type: Portability");
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        FU_CORE_ERROR("Type: Performance");
        break;
    case GL_DEBUG_TYPE_MARKER:
        FU_CORE_ERROR("Type: Marker");
        break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
        FU_CORE_ERROR("Type: Push Group");
        break;
    case GL_DEBUG_TYPE_POP_GROUP:
        FU_CORE_ERROR("Type: Pop Group");
        break;
    case GL_DEBUG_TYPE_OTHER:
        FU_CORE_ERROR("Type: Other");
        break;
    }

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        FU_CORE_ERROR("Severity: High");
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        FU_CORE_ERROR("Severity: Medium");
        break;
    case GL_DEBUG_SEVERITY_LOW:
        FU_CORE_ERROR("Severity: Low");
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        FU_CORE_ERROR("Severity: Notification");
        break;
    }
   
        FU_CORE_ERROR("Message: {0}, Source: {1}, Type: {2}, ID: {3}, Severity: {4}\n",
            (const char*)message, source, type, id, severity);
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
