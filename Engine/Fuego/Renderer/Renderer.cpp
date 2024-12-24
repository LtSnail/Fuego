#include "Renderer.h"

#include <span>

#include "CommandBuffer.h"
#include "Surface.h"

namespace Fuego::Renderer
{

Renderer::Renderer()
{
    _device = Device::CreateDevice();
    _commandQueue = _device->CreateCommandQueue();
    _commandPool = _device->CreateCommandPool(*_commandQueue);

    // Temporary: we're creating surface for main Application window
    _surface = _device->CreateSurface(Fuego::Application::Get().GetWindow().GetNativeHandle());
    _swapchain = _device->CreateSwapchain(*_surface);

    _vertexShader = _device->CreateShader("vs_shader", Shader::ShaderType::Vertex);
    _pixelShader = _device->CreateShader("ps_triangle", Shader::ShaderType::Pixel);
}

void Renderer::DrawMesh(float mesh[], uint32_t vertexCount, uint32_t stride)
{
    static std::unique_ptr<Buffer> vertexBuffer = _device->CreateBuffer(vertexCount * stride, 0);
    vertexBuffer->BindData<float>(std::span(mesh, vertexCount * (stride / sizeof(float))));

    std::unique_ptr<CommandBuffer> cmd = _commandPool->CreateCommandBuffer();
    cmd->BindRenderTarget(_swapchain->GetScreenTexture());
    cmd->BindVertexBuffer(*vertexBuffer);
    cmd->BindVertexShader(*_vertexShader);
    cmd->BindPixelShader(*_pixelShader);
    cmd->Draw(3);
}

void Renderer::Clear()
{
    std::unique_ptr<CommandBuffer> cmd = _commandPool->CreateCommandBuffer();
    cmd->Clear();
}

void Renderer::Present()
{
    _swapchain->Present();
}

}  // namespace Fuego::Renderer
