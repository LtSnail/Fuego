#include "OpenGLContext.h"

// clang-format off
#include <glad/glad.h>
#include "fupch.h"

namespace Fuego
{
OpenGLContext::OpenGLContext(HWND* windowHandle)
    : _windowHandle(windowHandle), _openGLContext(nullptr)
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
    _openGLContext = wglCreateContext(hdc);

        if (!::gladLoadGLLoader((GLADloadproc)::wglGetProcAddress))
    {
        FU_CORE_CRITICAL("[Glad hasn't been initialized!]");
        return false;
    }

    if (!wglMakeCurrent(hdc, _openGLContext))
    {
        FU_CORE_CRITICAL("[Failed to make OpenGL context current!]");
        return false;
    }



    return true;
}
void OpenGLContext::SwapBuffers()
{
    HDC hdc = GetDC(*_windowHandle);
    ::SwapBuffers(hdc);
}
}  // namespace Fuego