#pragma once

#include "Renderer/Device.h"

#include <Metal/Metal.hpp>


namespace Fuego::Renderer
{
class DeviceMetal : public Device
{
    
public:
    DeviceMetal();
    ~DeviceMetal();

    virtual std::unique_ptr<Buffer> CreateBuffer(size_t size, uint32_t flags) override;
    virtual std::unique_ptr<CommandQueue> CreateQueue() override;
    virtual std::unique_ptr<CommandPool> CreateCommandPool(std::shared_ptr<CommandQueue> queue) override;
    virtual std::unique_ptr<Swapchain> CreateSwapchain(std::shared_ptr<Surface> surface) override;
    virtual std::unique_ptr<Shader> CreateShader(std::string_view shaderName) override;
    
private:
    MTL::Device* _device;
    MTL::Library* _defaultLibrary;
};
}  // namespace Fuego::Renderer
