#pragma once

#include <Fuego.h>

class SceneLayer final : public Fuego::Layer
{
public:
    SceneLayer();

    virtual void OnUpdate() override;
    virtual void OnAttach() override;
    virtual void OnDetach() override;

    virtual void OnEvent(Fuego::EventVariant& event) override;
    bool OnRenderEvent(Fuego::AppRenderEvent& event);
};

class SandboxApp : public Fuego::Application
{
    SandboxApp();
};
