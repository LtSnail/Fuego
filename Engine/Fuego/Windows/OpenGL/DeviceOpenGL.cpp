#include "OpenGL/DeviceOpenGL.h"

#include "Application.h"
#include "Core.h"
#include "glad/glad.h"


namespace Fuego::Renderer
{
DeviceOpenGL::DeviceOpenGL()
    : _hwnd(nullptr)
    , _ctx(nullptr)
    , _hdc(nullptr)
{
    _hwnd = static_cast<HWND*>(Application().GetWindow().GetNativeWindow());

    // clang-format off
    _pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        32,
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        24,
        8,
        0,
        PFD_MAIN_PLANE,
        0,
        0, 0, 0};
    // clang-format on

    _hdc = GetDC(*_hwnd);
    int pixelFormat = ChoosePixelFormat(_hdc, &_pfd);
    SetPixelFormat(_hdc, pixelFormat, &_pfd);
    _ctx = wglCreateContext(_hdc);
    wglMakeCurrent(_hdc, _ctx);
    if (!gladLoadGL())
        FU_CORE_ASSERT(_ctx, "[OpenGL] hasn't been initialized!");

    FU_CORE_INFO("OpenGL info:");
    FU_CORE_INFO("  Version: {0}", (const char*)glGetString(GL_VERSION));
    FU_CORE_INFO("  GLSL Version: {0}", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
    FU_CORE_INFO("  GPU Vendor: {0}", (const char*)glGetString(GL_VENDOR));
    FU_CORE_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
}

DeviceOpenGL::~DeviceOpenGL()
{
    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(_ctx);
}

std::unique_ptr<Buffer> DeviceOpenGL::CreateBuffer(size_t size, uint32_t flags)
{
    UNUSED(size);
    UNUSED(flags);

    return nullptr;
}

std::unique_ptr<CommandQueue> DeviceOpenGL::CreateQueue()
{
    return nullptr;
}

std::unique_ptr<CommandPool> DeviceOpenGL::CreateCommandPool(std::shared_ptr<CommandQueue> queue)
{
    UNUSED(queue);

    return nullptr;
}

std::unique_ptr<Swapchain> DeviceOpenGL::CreateSwapchain(std::shared_ptr<Surface> surface)
{
    UNUSED(surface);

    return nullptr;
}

std::unique_ptr<Device> DeviceOpenGL::CreateDevice(Surface* surface)
{
    return std::make_unique<DeviceOpenGL>(surface);
}
}  // namespace Fuego::Renderer
