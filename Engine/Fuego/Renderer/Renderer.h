#pragma once

#include "Application.h"
#include "Buffer.h"
#include "CommandPool.h"
#include "CommandQueue.h"
#include "Device.h"
#include "Shader.h"
#include "Swapchain.h"

namespace Fuego::Renderer
{
class Surface;

class Renderer
{
public:
    ~Renderer() = default;
    // TODO replace array of floats to Mesh class
    void DrawMesh(float vertices[], uint32_t vertexCount, uint32_t indices[], uint32_t indicesCount);
    void Clear();
    void Present();

    Renderer(const Renderer&&) = delete;
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

private:
    std::unique_ptr<Buffer> _buffer;
    std::unique_ptr<Device> _device;
    std::unique_ptr<CommandQueue> _commandQueue;
    std::unique_ptr<CommandPool> _commandPool;
    std::unique_ptr<Swapchain> _swapchain;
    std::unique_ptr<Shader> _vertexShader;
    std::unique_ptr<Shader> _pixelShader;
    std::unique_ptr<Surface> _surface;

    friend class Fuego::Application;
    Renderer();
};
}  // namespace Fuego::Renderer
