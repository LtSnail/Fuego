#pragma once

namespace Fuego::Renderer
{
class Buffer;
class CommandQueue;
class CommandPool;
class Swapchain;
class Surface;
class Shader;

class Device
{
public:
    virtual ~Device() = default;

    virtual std::unique_ptr<Buffer> CreateBuffer(size_t size, uint32_t flags) = 0;
    virtual std::unique_ptr<CommandQueue> CreateQueue() = 0;
    virtual std::unique_ptr<CommandPool> CreateCommandPool(const CommandQueue& queue) = 0;
    virtual std::unique_ptr<Swapchain> CreateSwapchain(const Surface& surface) = 0;
    virtual std::unique_ptr<Shader> CreateShader(std::string_view shaderName) = 0;

    static std::unique_ptr<Device> CreateDevice(const Surface& surface);
};
}  // namespace Fuego::Renderer
