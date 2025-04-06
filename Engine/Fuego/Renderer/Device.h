#pragma once

#include "Shader.h"

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
    static std::unique_ptr<Device> CreateDevice();
    virtual ~Device() = default;

    virtual std::unique_ptr<Buffer> CreateBuffer(size_t size, uint32_t flags) = 0;
    virtual std::unique_ptr<CommandQueue> CreateCommandQueue() = 0;
    virtual std::unique_ptr<CommandPool> CreateCommandPool(const CommandQueue& queue) = 0;

    virtual std::unique_ptr<Swapchain> CreateSwapchain(const Surface& surface) = 0;

    virtual std::unique_ptr<Shader> CreateShader(std::string_view shaderName, Shader::ShaderType type) = 0;
    virtual std::unique_ptr<Surface> CreateSurface(const void* window) = 0;
    virtual std::unique_ptr<Texture> CreateTexture(unsigned char* buffer, int width, int height) = 0;
};
}  // namespace Fuego::Renderer
