#pragma once

#include "EventQueue.h"
#include "Events/ApplicationEvent.h"
#include "Events/EventVisitor.h"
#include "FileSystem/FileSystem.h"
#include "Layer.h"
#include "Window.h"


namespace Fuego::FS
{
class FileSystem;
}

namespace Fuego::Renderer
{
class Renderer;
class Model;
class Texture;
}  // namespace Fuego::Renderer

namespace Fuego
{

class FUEGO_API Application
{
    FUEGO_INTERFACE(Application)

public:
    Application();
    virtual ~Application();
    FUEGO_NON_COPYABLE_NON_MOVABLE(Application)

    void Run();

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* overlay);

    void OnEvent(EventVariant& event);
    bool OnRenderEvent(AppRenderEvent& event);

    // Window events:
    bool OnWindowClose(WindowCloseEvent& event);
    bool OnWindowResize(WindowResizeEvent& event);
    bool OnStartResizeWindow(WindowStartResizeEvent& event);
    bool OnEndResizeWindow(WindowEndResizeEvent& event);
    bool OnValidateWindow(WindowValidateEvent& event);
    bool OnKeyPressEvent(KeyPressedEvent& event);

    // Input events
    bool OnMouseMoveEvent(MouseMovedEvent& event);

    static Application& Get();
    Fuego::Renderer::Renderer& Renderer();
    Fuego::FS::FileSystem& FileSystem();
    Window& GetWindow();

    Fuego::Renderer::Model* LoadModel(std::string_view path);

    bool IsTextureLoaded(std::string_view) const;
    bool AddTexture(std::string_view);
    const Fuego::Renderer::Texture* GetLoadedTexture(std::string_view name) const;
};

// Should be defined in a client.
Application* CreateApplication();

}  // namespace Fuego
