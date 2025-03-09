#include "SandboxApp.h"

#include "Camera.h"


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
        mesh_data.push_back(std::move(mesh->load(fs.GetFullPathToFile("Model.obj").data())));
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
        Fuego::EventVisitor{[this](Fuego::AppRenderEvent& ev) { OnRenderEvent(ev); }, [](const Fuego::Event& ev) { /*FU_TRACE("{0}", ev.ToString());*/ }};

    std::visit(LogEventVisitor, event);
}

bool SceneLayer::OnRenderEvent(Fuego::AppRenderEvent& event)
{
    // glm::mat4 model_pos = glm::translate(glm::mat4(1.0f), glm::vec3(15.0f, 0.0f, -5.0f));
    // glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1280.0F / 720.0F, 0.1f, 100.0f);
    //// FU_TRACE("Client OnRenderEvent");
    // auto& renderer = Fuego::Application::Get().Renderer();
    // int i = 0;
    // for (auto& mesh : mesh_data)
    //{
    //     renderer.DrawMesh(mesh, scene_meshes[i]->GetVertexCount(), texture_data, w, h, model_pos,
    //                       /* Fuego::Renderer::Camera::GetActiveCamera()->GetView(),*/ glm::mat4(1.0f), projection);
    //     i++;
    // }
    UNUSED(event);
    return true;
}
