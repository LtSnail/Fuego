#include "OpenGLContext.h"

namespace Fuego
{
OpenGLContext::OpenGLContext(HWND* windowHandle)
    : _windowHandle(windowHandle)
    , _openGLContext(nullptr)
{
    FU_CORE_ASSERT(_windowHandle, "OpenGL context is nullptr");
}

OpenGLContext::~OpenGLContext()
{
    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(_openGLContext);
}

bool OpenGLContext::Init()
{
    HDC hdc = GetDC(*_windowHandle);

    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int pixelFormat = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, pixelFormat, &pfd);

    _openGLContext = wglCreateContext(hdc);

    wglMakeCurrent(hdc, _openGLContext);

    if (!gladLoadGL())
    {
        FU_CORE_CRITICAL("[OpenGL] hasn't been initialized!");
        return false;
    }
    FU_CORE_INFO("OpenGL info:");
    FU_CORE_INFO("  Version: {0}", (const char*)glGetString(GL_VERSION));
    FU_CORE_INFO("  GLSL Version: {0}", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
    FU_CORE_INFO("  GPU Vendor: {0}", (const char*)glGetString(GL_VENDOR));
    FU_CORE_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));

    return true;
}
void OpenGLContext::SwapBuffers()
{
    HDC hdc = GetDC(*_windowHandle);
    ::SwapBuffers(hdc);
}
}  // namespace Fuego
