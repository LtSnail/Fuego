#pragma once

#include "Shader.h"

namespace Fuego::Graphics
{
class Buffer;
class CommandQueue;
class CommandPool;
class CommandBuffer;
class Swapchain;
class Surface;
class Shader;

class Device
{
public:
    static std::unique_ptr<Device> CreateDevice();
    virtual ~Device() = default;

    virtual std::unique_ptr<Buffer> CreateBuffer(size_t size, uint32_t flags) = 0;
    virtual std::unique_ptr<CommandQueue> CreateCommandQueue() = 0;
    virtual std::unique_ptr<CommandPool> CreateCommandPool(const CommandQueue& queue) = 0;
    virtual std::unique_ptr<CommandBuffer> CreateCommandBuffer() = 0;

    virtual std::unique_ptr<Swapchain> CreateSwapchain(const Surface& surface) = 0;

    virtual std::unique_ptr<Surface> CreateSurface(const void* window) = 0;
    virtual std::unique_ptr<Texture> CreateTexture(unsigned char* buffer, int width, int height) = 0;

    // Yes, I know, raw pointer, so be carefull here
    virtual Shader* CreateShader(std::string_view shaderName, Shader::ShaderType type) = 0;

    virtual void Release() = 0;
};
}  // namespace Fuego::Graphics
