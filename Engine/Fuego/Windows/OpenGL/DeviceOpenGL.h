﻿#pragma once

#include "Renderer/Device.h"

namespace Fuego::Renderer
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

    virtual std::unique_ptr<Shader> CreateShader(std::string_view shaderName, Shader::ShaderType type) override;
    virtual std::unique_ptr<Surface> CreateSurface(const void* window) override;
    virtual std::unique_ptr<Texture> CreateTexture(unsigned char* buffer, int width, int height) override;

protected:
    friend class Device;
    DeviceOpenGL();

private:
    HGLRC ctx;
    int max_textures_units;
};
}  // namespace Fuego::Renderer
