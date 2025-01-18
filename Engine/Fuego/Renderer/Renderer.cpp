#include "Renderer.h"

#include <span>

#include "Camera.h"
#include "CommandBuffer.h"
#include "Mesh.h"


namespace Fuego::Renderer
{

Renderer::Renderer()
    : show_wireframe(false)
{
    _camera = std::unique_ptr<Camera>(new Camera());
    _camera->Activate();

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
    cmd.IndexedDraw(vertexCount);
    cmd.EndRecording();
    cmd.Submit();
}

void Renderer::DrawMesh(const std::vector<float>& data, uint32_t vertex_count)
{
    _buffer->BindData<float>(std::span(data.data(), data.size()));
    CommandBuffer& cmd = _commandPool->GetCommandBuffer();
    cmd.BeginRecording();
    cmd.BindRenderTarget(_swapchain->GetScreenTexture());
    cmd.BindVertexBuffer(*_buffer);
    cmd.BindVertexShader(*_vertexShader);
    cmd.BindPixelShader(*_pixelShader);
    // cmd.BindIndexBuffer( );
    // cmd.IndexedDraw();
    cmd.Draw(vertex_count);
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

void Renderer::ShowWireFrame()
{
    if (show_wireframe)
    {
        _swapchain->ShowWireFrame(true);
    }
    else
    {
        _swapchain->ShowWireFrame(false);
    }
}

void Renderer::ToggleWireFrame()
{
    show_wireframe = !show_wireframe;
}

void Renderer::ValidateWindow()
{
    _swapchain->ValidateWindow();
}

void Renderer::ChangeViewport(float x, float y, float w, float h)
{
    viewport.x = x;
    viewport.y = y;
    viewport.width = w;
    viewport.heigth = h;
    UpdateViewport();
}

void Renderer::UpdateViewport()
{
    _surface.release();
    _surface = _device->CreateSurface(Fuego::Application::Get().GetWindow().GetNativeHandle());
    _swapchain.release();
    _swapchain = _device->CreateSwapchain(*_surface, 0, 0, viewport.width, viewport.heigth);
}

}  // namespace Fuego::Renderer
