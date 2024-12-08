#include "OpenGL/DeviceOpenGL.h"

#include "Application.h"
#include "BufferOpenGL.h"
#include "CommandPoolOpenGl.h"
#include "CommandQueueOpenGL.h"
#include "Core.h"
#include "ShaderOpenGL.h"
#include "SurfaceWindows.h"
#include "SwapchainOpenGL.h"
#include "glad/glad.h"


namespace Fuego::Renderer
{
DeviceOpenGL::DeviceOpenGL(const Surface& surface)
    : _ctx(nullptr)
{
    const SurfaceWindows& surfaceWin = dynamic_cast<const SurfaceWindows&>(surface);
    const HDC hdc = surfaceWin.GetHDC();
    int pixelFormat = ChoosePixelFormat(hdc, surfaceWin.GetPFD());
    SetPixelFormat(hdc, pixelFormat, surfaceWin.GetPFD());
    _ctx = wglCreateContext(hdc);
    wglMakeCurrent(hdc, _ctx);
    if (!gladLoadGL())
        FU_CORE_ASSERT(_ctx, "[OpenGL] hasn't been initialized!");

    FU_CORE_INFO("OpenGL info:");
    FU_CORE_INFO("  Version: {0}", (const char*)glGetString(GL_VERSION));
    FU_CORE_INFO("  GLSL Version: {0}", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
    FU_CORE_INFO("  GPU Vendor: {0}", (const char*)glGetString(GL_VENDOR));
    FU_CORE_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
}

std::unique_ptr<Device> Device::CreateDevice(const Surface& surface)
{
    return std::make_unique<DeviceOpenGL>(surface);
}

DeviceOpenGL::~DeviceOpenGL()
{
    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(_ctx);
}

std::unique_ptr<Buffer> DeviceOpenGL::CreateBuffer(size_t size, uint32_t flags)
{
    return Buffer::Create(size, flags);
}

std::unique_ptr<CommandQueue> DeviceOpenGL::CreateQueue()
{
    return CommandQueue::CreateQueue();
}

std::unique_ptr<CommandPool> DeviceOpenGL::CreateCommandPool(const CommandQueue& queue)
{
    return CommandPoolOpenGl::CreateCommandPool(queue);
}

std::unique_ptr<Swapchain> DeviceOpenGL::CreateSwapchain(const Surface& surface)
{
    return SwapchainOpenGL::CreateSwapChain(surface);
}

std::unique_ptr<Shader> DeviceOpenGL::CreateShader(std::string_view shaderName)
{
    return ShaderOpenGL::CreateShader(nullptr, Shader::ShaderType::None);
}

}  // namespace Fuego::Renderer
