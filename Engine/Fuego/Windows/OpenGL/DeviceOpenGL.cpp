#include "OpenGL/DeviceOpenGL.h"

#include "Application.h"
#include "BufferOpenGL.h"
#include "CommandPoolOpenGL.h"
#include "CommandQueueOpenGL.h"
#include "ShaderOpenGL.h"
#include "SwapchainOpenGL.h"
// clang-format off
#include "glad/wgl.h"
#include "glad/gl.h"
// clang-format on

namespace Fuego::Renderer
{

DeviceOpenGL::DeviceOpenGL()
    : ctx(nullptr)
{
    HWND hwnd = (HWND)Application::Get().GetWindow().GetNativeHandle();
    HDC hdc = GetDC(hwnd);

    int pixel_format_attribs[] = {WGL_DRAW_TO_WINDOW_ARB,
                                  GL_TRUE,
                                  WGL_SUPPORT_OPENGL_ARB,
                                  GL_TRUE,
                                  WGL_DOUBLE_BUFFER_ARB,
                                  GL_TRUE,
                                  WGL_ACCELERATION_ARB,
                                  WGL_FULL_ACCELERATION_ARB,
                                  WGL_PIXEL_TYPE_ARB,
                                  WGL_TYPE_RGBA_ARB,
                                  WGL_COLOR_BITS_ARB,
                                  32,
                                  WGL_DEPTH_BITS_ARB,
                                  24,
                                  WGL_STENCIL_BITS_ARB,
                                  8,
                                  0};
    int pixel_format;
    UINT num_formats;
    int res = wglChoosePixelFormatARB(hdc, pixel_format_attribs, 0, 1, &pixel_format, &num_formats);
    if (!res)
    {
        FU_CORE_ERROR("Failed to choose pixel format");
    }

    if (!num_formats)
        FU_CORE_ERROR("Failed to set the OpenGL 4.6 pixel format.");

    PIXELFORMATDESCRIPTOR pfd;
    DescribePixelFormat(hdc, pixel_format, sizeof(pfd), &pfd);
    if (!SetPixelFormat(hdc, pixel_format, &pfd))
        FU_CORE_ERROR("Failed to set the OpenGL 4.6 pixel format.");

    // clang-format off
    int ctx_attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB,
        4,
        WGL_CONTEXT_MINOR_VERSION_ARB,
        6,
        WGL_CONTEXT_PROFILE_MASK_ARB,
        WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0,
    };
    // clang-format on

    ctx = wglCreateContextAttribsARB(hdc, 0, ctx_attribs);
    if (!ctx)
        FU_CORE_ERROR("Failed to create OpenGL 4.6 context.");

    if (!wglMakeCurrent(hdc, ctx))
        FU_CORE_ERROR("Failed to activate OpenGL 4.6 rendering context.");

    if (!gladLoaderLoadGL())
        FU_CORE_ERROR("[OpenGL] can't load OoenGL");

    FU_CORE_INFO("OpenGL info:");
    FU_CORE_INFO("  Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    FU_CORE_INFO("  GLSL Version: {0}", reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
    FU_CORE_INFO("  GPU Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
    FU_CORE_INFO("  Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    // TODO: if debug then enable OpenGL debug callback:
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(DeviceOpenGL::OpenGLDebugCallbackFunc, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

    glEnable(GL_DEPTH_TEST);
}

void DeviceOpenGL::OpenGLDebugCallbackFunc(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
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

    FU_CORE_ERROR("Message: {0}, Source: {1}, Type: {2}, ID: {3}, Severity: {4}\n", (const char*)message, source, type, id, severity);
}

DeviceOpenGL::~DeviceOpenGL()
{
    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(ctx);
}

std::unique_ptr<Device> Device::CreateDevice()
{
    return std::unique_ptr<Device>(new DeviceOpenGL());
}

std::unique_ptr<Surface> Device::CreateSurface(const void* window)
{
    return std::unique_ptr<Surface>(new SurfaceOpenGL(window));
}


std::unique_ptr<Buffer> DeviceOpenGL::CreateBuffer(size_t size, uint32_t flags)
{
    return std::unique_ptr<Buffer>(new BufferOpenGL());
}

std::unique_ptr<CommandQueue> DeviceOpenGL::CreateCommandQueue()
{
    return std::unique_ptr<CommandQueue>(new CommandQueueOpenGL());
}

std::unique_ptr<CommandPool> DeviceOpenGL::CreateCommandPool(const CommandQueue& queue)
{
    return std::unique_ptr<CommandPoolOpenGL>(new CommandPoolOpenGL(queue));
}

std::unique_ptr<Swapchain> DeviceOpenGL::CreateSwapchain(const Surface& surface)
{
    return std::unique_ptr<Swapchain>(new SwapchainOpenGL(surface));
}

std::unique_ptr<Shader> DeviceOpenGL::CreateShader(std::string_view shaderName, Shader::ShaderType type)
{
    const std::string shaderCode = Application::Get().FileSystem().OpenFile(std::string(shaderName) + ".glsl");
    return std::unique_ptr<Shader>(new ShaderOpenGL(shaderCode.c_str(), type));
}


}  // namespace Fuego::Renderer
