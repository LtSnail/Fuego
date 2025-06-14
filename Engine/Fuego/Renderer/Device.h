#pragma once

#include "Shader.h"

namespace Fuego::Graphics
{
enum class TextureFormat;
class Buffer;
class CommandQueue;
class CommandPool;
class CommandBuffer;
class Swapchain;
class Surface;
class Shader;
class Color;
enum RenderStage;

class Device
{
   public:
    static std::unique_ptr<Device> CreateDevice();
    virtual ~Device() = default;

    virtual std::unique_ptr<Buffer> CreateBuffer(RenderStage stage, size_t size) = 0;
    virtual std::unique_ptr<CommandQueue> CreateCommandQueue() = 0;
    virtual std::unique_ptr<CommandPool> CreateCommandPool(const CommandQueue& queue) = 0;
    virtual std::unique_ptr<CommandBuffer> CreateCommandBuffer() = 0;

    virtual std::unique_ptr<Swapchain> CreateSwapchain(const Surface& surface) = 0;

    virtual std::unique_ptr<Surface> CreateSurface(const void* window) = 0;

    virtual std::shared_ptr<Texture> CreateTexture(std::string_view name, TextureFormat format, unsigned char* buffer,
                                                   int width, int height) const = 0;

    virtual std::shared_ptr<Texture> CreateTexture(std::string_view name) const = 0;

    virtual std::shared_ptr<Texture> CreateTexture(std::string_view name, TextureFormat format, Color color, int width,
                                                   int height) const = 0;

    // Yes, I know, raw pointer, so be careful here
    virtual Shader* CreateShader(std::string_view shaderName, Shader::ShaderType type) = 0;

    virtual void SetVSync(bool active) const = 0;

    virtual void Release() = 0;
};
}  // namespace Fuego::Graphics
