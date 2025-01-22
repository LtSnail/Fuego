#include "SandboxApp.h"


Fuego::Application* Fuego::CreateApplication()
{
    return new SandboxApp();
}

ExampleLayer::ExampleLayer()
        : Layer("Example")
    {
    }

void ExampleLayer::OnUpdate() 
{
}

void ExampleLayer::OnEvent(Fuego::EventVariant& event)
    {
        auto LogEventVisitor =
            Fuego::EventVisitor{[this](Fuego::AppRenderEvent& ev) { OnRenderEvent(ev); },
            [](const Fuego::Event& ev) 
            {
                FU_TRACE("{0}", ev.ToString()); 
            }};

        std::visit(LogEventVisitor, event);
    }

bool ExampleLayer::OnRenderEvent(Fuego::AppRenderEvent& event)
{
    UNUSED(event);
    FU_TRACE("Client OnRenderEvent");
    return true;
}

SandboxApp::SandboxApp()
{
    PushLayer(new ExampleLayer);
}
