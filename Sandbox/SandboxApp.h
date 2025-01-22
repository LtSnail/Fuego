#pragma once

#include <Fuego.h>

class ExampleLayer : public Fuego::Layer
{
public:
    ExampleLayer();

    virtual void OnUpdate();
    virtual void OnEvent(Fuego::EventVariant& event) override;
    bool OnRenderEvent(Fuego::AppRenderEvent& event);
    
};

class SandboxApp : public Fuego::Application
{
public:
    SandboxApp();
};

Fuego::Application* Fuego::CreateApplication();

