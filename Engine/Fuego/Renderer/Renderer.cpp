#include "Renderer.h"

#include <span>

std::list<std::pair<std::shared_ptr<Fuego::Graphics::Image2D>, std::shared_ptr<Fuego::Graphics::Texture>>>*
    Fuego::Pipeline::PostLoadPipeline::pairs_ptr = nullptr;
std::list<std::pair<std::shared_ptr<Fuego::Graphics::Image2D>, std::shared_ptr<Fuego::Graphics::Texture>>>
    Fuego::Pipeline::Toolchain::renderer::pairs;

namespace Fuego::Graphics
{

ShaderObject* shader_object;

uint32_t Renderer::MAX_TEXTURES_COUNT = 0;

Renderer::Renderer(GraphicsAPI api, Fuego::Pipeline::Toolchain::renderer& toolchain)
    : show_wireframe(false)
    , _camera(nullptr)
    , current_shader_obj(nullptr)
    , is_vsync(true)
    , renderer(api)
    , toolchain(toolchain)
{
}

std::shared_ptr<Fuego::Graphics::Texture> Renderer::load_texture(std::string_view path)
{
    if (path.empty())
        return GetLoadedTexture("fallback");

    // Try to find first across loaded textures:
    std::string name = std::filesystem::path(path.data()).stem().string();
    auto it = textures.find(name);
    if (it != textures.end())
        return it->second;

    // If not, load Image and then create new texture:
    std::shared_ptr<Fuego::Graphics::Image2D> image{nullptr};
    auto assets_manager = ServiceLocator::instance().GetService<AssetsManager>();
    auto existing_img = assets_manager->Get<Fuego::Graphics::Image2D>(name);
    if (!existing_img.expired())
        image = existing_img.lock();
    else
    {  // TODO rewrite this method because we use handles ? maybe
        image = assets_manager->Load<Fuego::Graphics::Image2D>(path)->Resource();
    }

    auto texture = toolchain.load_texture(image, _device.get());
    auto emplaced_texture = textures.emplace(image->Name(), texture);
    return emplaced_texture.first->second;
}
std::shared_ptr<Fuego::Graphics::Texture> Renderer::load_texture(std::shared_ptr<Fuego::Graphics::Image2D> img)
{
    if (!img)
        return GetLoadedTexture("fallback");

    std::string name = std::filesystem::path(img->Name()).stem().string();
    auto it = textures.find(name);
    if (it != textures.end())
        return it->second;

    auto texture = toolchain.load_texture(img, _device.get());
    auto emplaced_texture = textures.emplace(img->Name(), texture);
    return emplaced_texture.first->second;
}

void Renderer::OnInit()
{
    _camera.reset(new Camera());
    _camera->Activate();

    _device = Device::CreateDevice();
    _device->SetVSync(is_vsync);

    _commandQueue = _device->CreateCommandQueue();

    // Temporary: we're creating surface for main Application window
    _surface = _device->CreateSurface(Fuego::Application::instance().GetWindow().GetNativeHandle());
    _swapchain = _device->CreateSwapchain(*_surface);
    _commandPool = _device->CreateCommandPool(*_commandQueue);

    opaque_shader.reset(
        ShaderObject::CreateShaderObject(_device->CreateShader("vs_shader", Shader::ShaderType::Vertex),
                                         _device->CreateShader("ps_triangle", Shader::ShaderType::Pixel)));
    opaque_shader->GetVertexShader()->AddVar("model");
    opaque_shader->GetVertexShader()->AddVar("view");
    opaque_shader->GetVertexShader()->AddVar("projection");
}

void Renderer::OnShutdown()
{
    _device->Release();
    _device.release();

    _surface->Release();
    _surface.release();

    opaque_shader->Release();
    opaque_shader.reset();
}

std::shared_ptr<Texture> Renderer::GetLoadedTexture(std::string_view path) const
{
    if (path.empty())
        return textures.find("fallback")->second;

    std::string name = std::filesystem::path(path.data()).stem().string();

    auto it = textures.find(name.data());
    if (it != textures.end())
        return it->second;
    else
        return textures.find("fallback")->second;
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
        cmd->PushDebugGroup(0, mesh->Name().data());
        current_shader_obj->BindMaterial(mesh->GetMaterial());
        cmd->IndexedDraw(mesh->GetIndicesCount(), (const void*)(mesh->GetIndexStart() * sizeof(uint32_t)));
        cmd->EndRecording();
        cmd->Submit();
        cmd->PopDebugGroup();
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

void Renderer::SetVSync(bool active)
{
    is_vsync = active;
    _device->SetVSync(is_vsync);
}

bool Renderer::IsVSync()
{
    return is_vsync;
}

void Renderer::OnUpdate(float dlTime)
{
    toolchain.update();
}

void Renderer::OnPostUpdate(float dlTime)
{
    // TODO
}

void Renderer::OnFixedUpdate()
{
    // TODO
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
    : pos(pos), textcoord(text_coord), normal(normal)
{
}

}  // namespace Fuego::Graphics
