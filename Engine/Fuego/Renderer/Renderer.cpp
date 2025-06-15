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

std::shared_ptr<Fuego::Graphics::Texture> Renderer::load_texture(std::string_view name, TextureFormat fmt, Color color,
                                                                 int width, int height)
{
    auto it = textures.find(name.data());
    if (it != textures.end())
        return it->second;

    auto emplaced_texture =
        textures.emplace(name, _device->CreateTexture(name, TextureFormat::R8, color, width, height));
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

    std::shared_ptr<ShaderObject> static_geometry_shader(
        ShaderObject::CreateShaderObject(_device->CreateShader("vs_shader", Shader::ShaderType::Vertex),
                                         _device->CreateShader("ps_triangle", Shader::ShaderType::Pixel)));

    static_geometry_shader->GetVertexShader()->AddVar("model");
    static_geometry_shader->GetVertexShader()->AddVar("view");
    static_geometry_shader->GetVertexShader()->AddVar("projection");

    static_geometry_cmd = _device->CreateCommandBuffer();
    static_geometry_cmd->BindShaderObject(static_geometry_shader);

    VertexLayout layout{};
    layout.AddAttribute(VertexLayout::VertexAttribute(0, 3, VertexLayout::DataType::FLOAT, true));
    layout.AddAttribute(VertexLayout::VertexAttribute(1, 2, VertexLayout::DataType::FLOAT, true));
    layout.AddAttribute(VertexLayout::VertexAttribute(2, 3, VertexLayout::DataType::FLOAT, true));
    static_geometry_cmd->BindVertexBuffer(
        _device->CreateBuffer(Fuego::Graphics::Buffer::BufferType::Vertex, STATIC_GEOMETRY, 100 * 1024 * 1024), layout);
    static_geometry_cmd->BindIndexBuffer(
        _device->CreateBuffer(Fuego::Graphics::Buffer::BufferType::Index, STATIC_GEOMETRY, 100 * 1024 * 1024));
}

void Renderer::OnShutdown()
{
    _device->Release();
    _device.release();

    _surface->Release();
    _surface.release();
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

void Renderer::DrawModel(RenderStage stage, const Model* model, glm::mat4 model_pos)
{
    switch (stage)
    {
        case STATIC_GEOMETRY:
        {
            auto it = static_geometry_models.find(model->GetName().data());
            if (it != static_geometry_models.end())
            {
                it->second.pos = model_pos;
                return;
            }

            DrawInfo draw{model, model_pos};

            draw.vertex_global_offset_bytes = static_geometry_cmd->UpdateBufferSubData<VertexData>(
                Buffer::Vertex, std::span(model->GetVerticesData(), model->GetVertexCount()));

            draw.index_global_offset_bytes = static_geometry_cmd->UpdateBufferSubData<uint32_t>(
                Buffer::Index, std::span(model->GetIndicesData(), model->GetIndicesCount()));

            static_geometry_models.emplace(model->GetName().data(), draw);
            static_geometry_models_vector.emplace_back(draw);
            break;
        }
        case DYNAMIC_DRAW:
            break;
    }
}

void Renderer::DrawQuad(const Shader* shader, const Texture* texture, uint32_t x, uint32_t y, uint32_t width,
                        uint32_t height) const
{
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

    // Main Pass
    static_geometry_cmd->PushDebugGroup(0, "[PASS] -> Main Pass");
    static_geometry_cmd->PushDebugGroup(0, "[STAGE] -> Static geometry stage");
    static_geometry_cmd->BeginRecording();
    static_geometry_cmd->BindRenderTarget(_swapchain->GetScreenTexture());

    static_geometry_cmd->ShaderObject()->Use();

    for (const auto& draw_info : static_geometry_models_vector)
    {
        static_geometry_cmd->PushDebugGroup(0, draw_info.model->GetName().data());
        static_geometry_cmd->ShaderObject()->GetVertexShader()->SetMat4f("model", draw_info.pos);
        static_geometry_cmd->ShaderObject()->GetVertexShader()->SetMat4f("view", _camera->GetView());
        static_geometry_cmd->ShaderObject()->GetVertexShader()->SetMat4f("projection", _camera->GetProjection());

        const auto* meshes = draw_info.model->GetMeshesPtr();

        uint32_t index_inner_offset_bytes = 0;
        for (const auto& mesh : *meshes)
        {
            static_geometry_cmd->PushDebugGroup(0, mesh->Name().data());
            static_geometry_cmd->ShaderObject()->BindMaterial(mesh->GetMaterial());
            static_geometry_cmd->IndexedDraw(mesh->GetIndicesCount(),
                                             draw_info.index_global_offset_bytes + index_inner_offset_bytes,
                                             draw_info.vertex_global_offset_bytes / sizeof(VertexData));

            index_inner_offset_bytes += mesh->GetIndicesCount() * sizeof(uint32_t);
            static_geometry_cmd->PopDebugGroup();
        }
        static_geometry_cmd->PopDebugGroup();
        static_geometry_cmd->EndRecording();
        static_geometry_cmd->Submit();
    }
    static_geometry_cmd->PopDebugGroup();
    static_geometry_cmd->PopDebugGroup();
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
