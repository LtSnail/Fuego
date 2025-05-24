#pragma once

#include "EventQueue.h"
#include "Events/ApplicationEvent.h"
#include "Events/EventVisitor.h"
#include "FileSystem/FileSystem.h"
#include "Layer.h"
#include "Services/ServiceLocator.h"
#include "Window.h"
#include "singleton.hpp"


namespace Fuego::FS
{
class FileSystem;
}

namespace Fuego::Graphics
{
class Renderer;
class Model;
class Texture;
}  // namespace Fuego::Graphics


namespace Fuego
{

class FUEGO_API Application : public singleton<Application>
{
    friend class singleton<Application>;

    FUEGO_INTERFACE(Application)

public:
    enum class RendererType
    {
        OpenGL = 0,
        Vulkan = 1,
    };

    struct ApplicationBootSettings
    {
        RendererType renderer = RendererType::OpenGL;
        bool vsync = false;
        WindowProps window_props = WindowProps{};
        float fixed_dt = 0.025f;
    };


    void Init(ApplicationBootSettings& settings);
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

    Window& GetWindow();

    void SetVSync(bool active) const;
    bool IsVSync() const;

protected:
    Application();
    virtual ~Application() override;
};
}  // namespace Fuego
