#pragma once
#include <memory>

#include "Renderer/Device.h"

namespace Fuego::Renderer
{
class DeviceOpenGL : public Device
{
public:
    virtual std::unique_ptr<Buffer> CreateBuffer(size_t size, uint32_t flags) override;
    virtual std::unique_ptr<CommandQueue> CreateQueue() override;
    virtual std::unique_ptr<CommandPool> CreateCommandPool(std::shared_ptr<CommandQueue> queue) override;
    virtual std::unique_ptr<Swapchain> CreateSwapchain(std::shared_ptr<Surface> surface) override;

    static std::unique_ptr<Device> CreateDevice();

protected:
    DeviceOpenGL();
};
}  // namespace Fuego::Renderer
