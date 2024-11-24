#include <Fuego.h>

class ExampleLayer : public Fuego::Layer
{
public:
    ExampleLayer()
        : Layer("Example")
    {
    }

    void OnUpdate() override
    {
    }

    void OnEvent(Fuego::Event& event) override
    {
        FU_TRACE("{0}", event.ToString());
    }
};

class SandboxApp : public Fuego::Application
{
public:
    SandboxApp()
    {
        PushLayer(new ExampleLayer);
    }
};

Fuego::Application* Fuego::CreateApplication()
{
    return new SandboxApp();
}
