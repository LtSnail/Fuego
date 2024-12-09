#pragma once
#include <memory>

#include "Renderer/Device.h"

namespace Fuego::Renderer
{
class DeviceOpenGL : public Device
{
public:
    DeviceOpenGL();
    virtual ~DeviceOpenGL() override;

    virtual std::unique_ptr<Buffer> CreateBuffer(size_t size, uint32_t flags) override;
    virtual std::unique_ptr<CommandQueue> CreateQueue() override;
    virtual std::unique_ptr<CommandPool> CreateCommandPool(const CommandQueue& queue) override;
    virtual std::unique_ptr<Swapchain> CreateSwapchain(const Surface& surface) override;
    virtual std::unique_ptr<Shader> CreateShader(std::string_view shaderName) override;

protected:
    friend class Device;
};
}  // namespace Fuego::Renderer
