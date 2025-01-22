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

    void OnEvent(Fuego::EventVariant& event) override
    {
        auto LogEventVisitor =
            Fuego::EventVisitor{[this](Fuego::AppRenderEvent& ev) { OnRenderEvent(ev); },
            [](const Fuego::Event& ev) 
            {
                FU_TRACE("{0}", ev.ToString()); 
            }};

        std::visit(LogEventVisitor, event);
    }

    bool OnRenderEvent(Fuego::AppRenderEvent& event)
    {
        UNUSED(event);
        FU_TRACE("Client OnRenderEvent");
        return true;
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
