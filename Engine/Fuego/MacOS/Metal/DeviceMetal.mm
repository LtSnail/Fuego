#include "Metal/DeviceMetal.h"
#include "Renderer/Device.h"

#include "Application.h"
#include "Core.h"


namespace Fuego::Renderer
{
DeviceMetal::DeviceMetal()
{
    
}

DeviceMetal::~DeviceMetal()
{

}

std::unique_ptr<Buffer> DeviceMetal::CreateBuffer(size_t size, uint32_t flags)
{
    UNUSED(size);
    UNUSED(flags);

    return nullptr;
}

std::unique_ptr<CommandQueue> DeviceMetal::CreateQueue()
{
    return nullptr;
}

std::unique_ptr<CommandPool> DeviceMetal::CreateCommandPool(std::shared_ptr<CommandQueue> queue)
{
    UNUSED(queue);

    return nullptr;
}

std::unique_ptr<Swapchain> DeviceMetal::CreateSwapchain(std::shared_ptr<Surface> surface)
{
    UNUSED(surface);

    return nullptr;
}

std::unique_ptr<Device> Device::CreateDevice()
{
    return std::make_unique<DeviceMetal>();
}
}  // namespace Fuego::Renderer
