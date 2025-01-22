#pragma once

#include <Metal/Metal.hpp>

#include "Renderer/Device.h"


namespace Fuego::Renderer
{
class DeviceMetal : public Device
{
public:
    DeviceMetal();
    ~DeviceMetal();

    virtual std::unique_ptr<Buffer> CreateBuffer(size_t size, uint32_t flags) override;
    virtual std::unique_ptr<CommandQueue> CreateCommandQueue() override;
    virtual std::unique_ptr<CommandPool> CreateCommandPool(const CommandQueue& queue) override;
    virtual std::unique_ptr<Swapchain> CreateSwapchain(const Surface& surface) override;
    virtual std::unique_ptr<Swapchain> CreateSwapchain(const Surface& surface, float x, float y, float w, float h) override;
    virtual std::unique_ptr<Shader> CreateShader(std::string_view shaderName, Shader::ShaderType type) override;
    virtual std::unique_ptr<Surface> CreateSurface(const void* window) override;

private:
    MTL::Device* _device;
    MTL::Library* _defaultLibrary;
};
}  // namespace Fuego::Renderer
