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
    _buffer = _device->CreateBuffer(0, 0);
}

void Renderer::DrawMesh(float vertices[], uint32_t vertexCount, uint32_t indices[], uint32_t indicesCount)
{
    _buffer->BindData<float>(std::span(vertices, vertexCount * sizeof(float)));

    CommandBuffer& cmd = _commandPool->GetCommandBuffer();
    cmd.BeginRecording();
    cmd.BindRenderTarget(_swapchain->GetScreenTexture());
    cmd.BindVertexBuffer(*_buffer);
    cmd.BindVertexShader(*_vertexShader);
    cmd.BindPixelShader(*_pixelShader);
    cmd.BindIndexBuffer(indices, sizeof(unsigned int) * indicesCount);
    cmd.IndexedDraw(vertexCount, indices);
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
