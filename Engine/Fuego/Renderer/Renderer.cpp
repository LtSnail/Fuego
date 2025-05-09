#include "Renderer.h"

#include <span>

namespace Fuego::Graphics
{

ShaderObject* shader_object;

uint32_t Renderer::MAX_TEXTURES_COUNT = 0;

Renderer::Renderer()
    : show_wireframe(false)
    , _camera(nullptr)
    , current_shader_obj(nullptr)
{
}

void Renderer::Init()
{
    if (is_initialized)
        return;

    _camera.reset(new Camera());
    _camera->Activate();

    _device = Device::CreateDevice();
    _commandQueue = _device->CreateCommandQueue();

    // Temporary: we're creating surface for main Application window
    _surface = _device->CreateSurface(Fuego::Application::instance().GetWindow().GetNativeHandle());
    _swapchain = _device->CreateSwapchain(*_surface);
    _commandPool = _device->CreateCommandPool(*_commandQueue);

    _mainVsShader = _device->CreateShader("vs_shader", Shader::ShaderType::Vertex);
    _pixelShader = _device->CreateShader("ps_triangle", Shader::ShaderType::Pixel);

    opaque_shader.reset(ShaderObject::CreateShaderObject(*_mainVsShader.get(), *_pixelShader.get()));
    opaque_shader->GetVertexShader()->AddVar("model");
    opaque_shader->GetVertexShader()->AddVar("view");
    opaque_shader->GetVertexShader()->AddVar("projection");
void Renderer::OnShutdown()
{
    _device->Release();
    _device.release();

    _surface->Release();
    _surface.release();

    is_initialized = true;
    opaque_shader->Release();
    opaque_shader.reset();
}

void Renderer::DrawModel(const Model* model, glm::mat4 model_pos)
{
    std::unique_ptr<CommandBuffer> command_buffer = _device->CreateCommandBuffer();
    CommandBuffer* cmd = command_buffer.get();
    cmd->PushDebugGroup(0, model->GetName().data());
    cmd->BeginRecording();
    cmd->BindRenderTarget(_swapchain->GetScreenTexture());

    VertexLayout layout{};
    layout.AddAttribute(VertexLayout::VertexAttribute(0, 3, VertexLayout::DataType::FLOAT, true));
    layout.AddAttribute(VertexLayout::VertexAttribute(1, 2, VertexLayout::DataType::FLOAT, true));
    layout.AddAttribute(VertexLayout::VertexAttribute(2, 3, VertexLayout::DataType::FLOAT, true));

    current_shader_obj->GetVertexShader()->SetMat4f("model", model_pos);
    current_shader_obj->GetVertexShader()->SetMat4f("view", _camera->GetView());
    current_shader_obj->GetVertexShader()->SetMat4f("projection", _camera->GetProjection());

    std::unique_ptr<Buffer> buffer = _device->CreateBuffer(0, 0);
    buffer->BindData<VertexData>(std::span(model->GetVerticesData(), model->GetVertexCount()));

    cmd->BindIndexBuffer(model->GetIndicesData(), model->GetIndicesCount() * sizeof(uint32_t));
    cmd->BindVertexBuffer(*buffer, layout);

    const auto* meshes = model->GetMeshesPtr();
    for (const auto& mesh : *meshes)
    {
        current_shader_obj->BindMaterial(mesh->GetMaterial());

        cmd->IndexedDraw(mesh->GetIndicesCount(), (const void*)(mesh->GetIndexStart() * sizeof(uint32_t)));
        cmd->EndRecording();
        cmd->Submit();
    }
}

void Renderer::Clear()
{
    // CommandBuffer& cmd = _commandPool->GetCommandBuffer();
    // cmd.Clear();
    _surface->Clear();
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

std::unique_ptr<Texture> Renderer::CreateTexture(unsigned char* buffer, int width, int height) const
{
    return _device->CreateTexture(buffer, width, height);
}

void Renderer::ChangeViewport(float x, float y, float w, float h)
{
    viewport.x = x;
    viewport.y = y;
    viewport.width = w;
    viewport.height = h;
    UpdateViewport();
}

void Renderer::UpdateViewport()
{
    _surface.release();
    _surface = _device->CreateSurface(Fuego::Application::instance().GetWindow().GetNativeHandle());
    _swapchain.release();
    _swapchain = _device->CreateSwapchain(*_surface);
}

VertexData::VertexData(glm::vec3 pos, glm::vec3 text_coord, glm::vec3 normal)
    : pos(pos)
    , textcoord(text_coord)
    , normal(normal)
{
}

}  // namespace Fuego::Graphics
