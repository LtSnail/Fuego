#include "OpenGL/DeviceOpenGL.h"

#include "Application.h"
#include "BufferOpenGL.h"
#include "CommandPoolOpenGL.h"
#include "CommandQueueOpenGL.h"
#include "ShaderOpenGL.h"
#include "SwapchainOpenGL.h"


namespace Fuego::Renderer
{
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
