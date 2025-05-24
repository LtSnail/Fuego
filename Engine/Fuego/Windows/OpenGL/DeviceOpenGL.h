#pragma once

#include "Renderer/Device.h"

namespace Fuego::Graphics
{
class DeviceOpenGL final : public Device
{
public:
    FUEGO_NON_COPYABLE_NON_MOVABLE(DeviceOpenGL)

    virtual ~DeviceOpenGL() override;

    virtual std::unique_ptr<Buffer> CreateBuffer(size_t size, uint32_t flags) override;
    virtual std::unique_ptr<CommandQueue> CreateCommandQueue() override;
    virtual std::unique_ptr<CommandPool> CreateCommandPool(const CommandQueue& queue) override;
    virtual std::unique_ptr<CommandBuffer> CreateCommandBuffer() override;

    virtual std::unique_ptr<Swapchain> CreateSwapchain(const Surface& surface) override;

    virtual Shader* CreateShader(std::string_view shaderName, Shader::ShaderType type) override;
    virtual std::unique_ptr<Surface> CreateSurface(const void* window) override;
    virtual std::unique_ptr<Texture> CreateTexture(std::string_view name, TextureFormat format, unsigned char* buffer, int width, int height) override;

    virtual void SetVSync(bool active) const override;

    virtual void Release() override;

protected:
    friend class Device;
    DeviceOpenGL();

private:
    HGLRC ctx;
    int max_textures_units;
};
}  // namespace Fuego::Graphics
