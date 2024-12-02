#include "OpenGL/DeviceOpenGL.h"

#include "Core.h"


namespace Fuego::Renderer
{
DeviceOpenGL::DeviceOpenGL()
{
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

std::unique_ptr<Device> DeviceOpenGL::CreateDevice()
{
    return std::make_unique<DeviceOpenGL>();
}
}  // namespace Fuego::Renderer
