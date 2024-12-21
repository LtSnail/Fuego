#include "Renderer.h"
#include "Surface.h"
#include "CommandBuffer.h"
#include <span>

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

void Renderer::DrawMesh(float vertices[], uint32_t vertexCount)
{
    static std::unique_ptr<Buffer> vertexBuffer = _device->CreateBuffer(vertexCount, 0);
    vertexBuffer->BindData<float>(std::span(vertices, vertexCount));

    std::unique_ptr<CommandBuffer> cmd = _commandPool->CreateCommandBuffer();
    cmd->BindRenderTarget(_swapchain->GetScreenTexture());
    cmd->BindVertexBuffer(*vertexBuffer);
    cmd->BindVertexShader(*_vertexShader);
    cmd->BindPixelShader(*_pixelShader);
    cmd->Draw(3);
}

void Renderer::Present()
{
}

}