#pragma once

#include <Fuego.h>
#include "Mesh.h"

class SceneLayer : public Fuego::Layer
{
public:
    SceneLayer();

    virtual void OnUpdate() override;
    virtual void OnAttach() override;
    virtual void OnDetach() override;

    virtual void OnEvent(Fuego::EventVariant& event) override;
    bool OnRenderEvent(Fuego::AppRenderEvent& event);

private:
    std::vector<Fuego::Renderer::Mesh*> scene_meshes;
    std::vector<std::vector<float>> mesh_data;
};

class SandboxApp : public Fuego::Application
{
public:
    SandboxApp();
};

Fuego::Application* Fuego::CreateApplication();

