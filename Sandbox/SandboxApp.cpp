#include "SandboxApp.h"


unsigned char* texture_data;
int w, h, n;

Fuego::Application* Fuego::CreateApplication()
{
    return new SandboxApp();
}

SandboxApp::SandboxApp()
{
    SceneLayer* scene_layer = new SceneLayer();
    PushLayer(scene_layer);
}

SceneLayer::SceneLayer()
    : Layer("3D scene layer")
{
}

void SceneLayer::OnUpdate()
{
}

void SceneLayer::OnAttach()
{
    scene_meshes.emplace_back(new Fuego::Renderer::Mesh());
    Fuego::FS::FileSystem& fs = Fuego::Application::Get().FileSystem();
    for (const auto& mesh : scene_meshes)
    {
        mesh_data.emplace_back((mesh->load(fs.GetFullPathTo("Model.obj").data())));
    }
    texture_data = fs.Load_Image("image.jpg", w, h, n);
}

void SceneLayer::OnDetach()
{
    scene_meshes.clear();
    mesh_data.clear();
}

void SceneLayer::OnEvent(Fuego::EventVariant& event)
{
    auto LogEventVisitor =
        Fuego::EventVisitor{[this](Fuego::AppRenderEvent& ev) { OnRenderEvent(ev); }, [](const Fuego::Event& ev) { FU_TRACE("{0}", ev.ToString()); }};

    std::visit(LogEventVisitor, event);
}

bool SceneLayer::OnRenderEvent(Fuego::AppRenderEvent& event)
{
    UNUSED(event);

    auto& renderer = Fuego::Application::Get().Renderer();
    int i = 0;
    for (auto& mesh : mesh_data)
    {
        renderer.DrawMesh(mesh, scene_meshes[i]->GetVertexCount(), texture_data, w, h);
        i++;
    }

    return true;
}
