#pragma once

#include <filesystem>

#include "EventQueue.h"
#include "Events/ApplicationEvent.h"
#include "Events/EventVisitor.h"
#include "Layer.h"
#include "LayerStack.h"
#include "Window.h"
#include "fstream"

namespace Fuego::Renderer
{
class Renderer;
}

namespace Fuego
{

class FUEGO_API Application
{
    FUEGO_INTERFACE(Application);

public:
    Application();
    virtual ~Application();
    FUEGO_NON_COPYABLE_NON_MOVABLE(Application)

    void Run();

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* overlay);

    void OnEvent(EventVariant& event);
    bool OnWindowClose(WindowCloseEvent& event);
    bool OnWindowResize(WindowResizeEvent& event);
    bool OnRenderEvent(AppRenderEvent& event);

    static Application& Get();
    Fuego::Renderer::Renderer& Renderer();
    Window& GetWindow();
};

// Should be defined in a client.
Application* CreateApplication();

const std::string pathToResources = std::filesystem::current_path().string() + "\\..\\..\\..\\Sandbox\\Resources\\";
const std::string pathToShadersWindows = "Windows\\Shaders\\";
std::string OpenFile(const std::string& file, std::fstream::ios_base::openmode mode = std::fstream::ios_base::in);

}  // namespace Fuego
