#include "SandboxApp.h"


Fuego::Application* Fuego::CreateApplication()
{
    return new SandboxApp();
}

SandboxApp::SandboxApp()
{
    PushLayer(new SceneLayer);
}

SceneLayer::SceneLayer()
        : Layer("Example")
    {
    }

void SceneLayer::OnUpdate()
{
}

void SceneLayer::OnAttach()
{
    scene_meshes.emplace_back(new Fuego::Renderer::Mesh());
    for (auto mesh : scene_meshes)
    {
        mesh_data.push_back(std::move(mesh->load(Fuego::Application::Get().FileSystem().GetFullPathTo("Model.obj").data())));
    }
}

void SceneLayer::OnDetach()
{
    for (auto mesh : scene_meshes)
    {
        delete mesh;
    }
    mesh_data.clear();
}

void SceneLayer::OnEvent(Fuego::EventVariant& event)
    {
        auto LogEventVisitor =
            Fuego::EventVisitor{[this](Fuego::AppRenderEvent& ev) { OnRenderEvent(ev); },
            [](const Fuego::Event& ev) 
            {
                FU_TRACE("{0}", ev.ToString()); 
            }};

        std::visit(LogEventVisitor, event);
    }

bool SceneLayer::OnRenderEvent(Fuego::AppRenderEvent& event)
    {
    UNUSED(event);
    FU_TRACE("Client OnRenderEvent");
    return true;
}


