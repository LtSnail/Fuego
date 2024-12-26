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

    // Temporary: we're creating surface for main Application window
    _surface = _device->CreateSurface(Fuego::Application::Get().GetWindow().GetNativeHandle());
    _swapchain = _device->CreateSwapchain(*_surface);
    _commandPool = _device->CreateCommandPool(*_commandQueue);

    _vertexShader = _device->CreateShader("vs_shader", Shader::ShaderType::Vertex);
    _pixelShader = _device->CreateShader("ps_triangle", Shader::ShaderType::Pixel);
}

void Renderer::DrawMesh(float mesh[], uint32_t vertexCount, uint32_t stride)
{
    static std::unique_ptr<Buffer> vertexBuffer = _device->CreateBuffer(vertexCount * stride, 0);
    vertexBuffer->BindData<float>(std::span(mesh, vertexCount * (stride / sizeof(float))));

    CommandBuffer& cmd = _commandPool->GetCommandBuffer();
    cmd.BeginRecording();
    cmd.BindRenderTarget(_swapchain->GetScreenTexture());
    cmd.BindVertexBuffer(*vertexBuffer);
    cmd.BindVertexShader(*_vertexShader);
    cmd.BindPixelShader(*_pixelShader);
    cmd.Draw(3);
    cmd.EndRecording();
    cmd.Submit();
}

void Renderer::Clear()
{
    CommandBuffer& cmd = _commandPool->GetCommandBuffer();
    cmd.Clear();
}

void Renderer::Present()
{
    _swapchain->Present();
}

}  // namespace Fuego::Renderer
