#include "Renderer.h"

#include <span>

#include "Camera.h"
#include "CommandBuffer.h"
#include "ShaderObject.h"
#include "VertexLayout.h"


namespace Fuego::Renderer
{

ShaderObject* shader_object;

uint32_t Renderer::MAX_TEXTURES_COUNT = 0;

Renderer::Renderer()
    : show_wireframe(false)
    , _camera(std::unique_ptr<Camera>(new Camera()))
{
    _camera->Activate();

    _device = Device::CreateDevice();
    _commandQueue = _device->CreateCommandQueue();

    // Temporary: we're creating surface for main Application window
    _surface = _device->CreateSurface(Fuego::Application::Get().GetWindow().GetNativeHandle());
    _swapchain = _device->CreateSwapchain(*_surface);
    _commandPool = _device->CreateCommandPool(*_commandQueue);

    _mainVsShader = _device->CreateShader("vs_shader", Shader::ShaderType::Vertex);
    _pixelShader = _device->CreateShader("ps_triangle", Shader::ShaderType::Pixel);

    shader_object = ShaderObject::CreateShaderObject(*_mainVsShader.get(), *_pixelShader.get());
    shader_object->GetVertexShader()->AddVar("model");
    shader_object->GetVertexShader()->AddVar("view");
    shader_object->GetVertexShader()->AddVar("projection");

    _buffer = _device->CreateBuffer(0, 0);
}

void Renderer::DrawMesh(float vertices[], uint32_t vertexCount, uint32_t indices[], uint32_t indicesCount)
{
    _buffer->BindData<float>(std::span(vertices, vertexCount * sizeof(float)));
    CommandBuffer& cmd = _commandPool->GetCommandBuffer();
    cmd.BeginRecording();
    cmd.BindRenderTarget(_swapchain->GetScreenTexture());
    VertexLayout layout{};
    layout.AddAttribute(VertexLayout::VertexAttribute(0, 3, VertexLayout::DataType::FLOAT, true));
    layout.AddAttribute(VertexLayout::VertexAttribute(1, 2, VertexLayout::DataType::FLOAT, true));
    layout.AddAttribute(VertexLayout::VertexAttribute(2, 3, VertexLayout::DataType::FLOAT, true));
    cmd.BindVertexBuffer(*_buffer, layout);
    cmd.BindIndexBuffer(indices, sizeof(unsigned int) * indicesCount);
    cmd.IndexedDraw(vertexCount);
    cmd.EndRecording();
    cmd.Submit();
}

void Renderer::DrawMesh(const std::vector<float>& data, uint32_t vertex_count, Material* material, glm::mat4 mesh_pos, glm::mat4 camera, glm::mat4 projection)
{
    _buffer->BindData<float>(std::span(data.data(), data.size()));
    CommandBuffer& cmd = _commandPool->GetCommandBuffer();
    cmd.BeginRecording();
    cmd.BindRenderTarget(_swapchain->GetScreenTexture());
    VertexLayout layout{};
    layout.AddAttribute(VertexLayout::VertexAttribute(0, 3, VertexLayout::DataType::FLOAT, true));
    layout.AddAttribute(VertexLayout::VertexAttribute(1, 2, VertexLayout::DataType::FLOAT, true));
    layout.AddAttribute(VertexLayout::VertexAttribute(2, 3, VertexLayout::DataType::FLOAT, true));
    cmd.BindVertexBuffer(*_buffer, layout);
    cmd.BindShaderObject(*shader_object);
    // cmd.BindTexture(texture);
    //  cmd.BindIndexBuffer( );
    //  cmd.IndexedDraw();
    shader_object->GetVertexShader()->SetMat4f("model", mesh_pos);
    shader_object->GetVertexShader()->SetMat4f("view", camera);
    shader_object->GetVertexShader()->SetMat4f("projection", projection);

    material->Upload(*shader_object);
    cmd.Draw(vertex_count);
    cmd.EndRecording();
    cmd.Submit();

    delete material;
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
    _swapchain = _device->CreateSwapchain(*_surface);
}

}  // namespace Fuego::Renderer
