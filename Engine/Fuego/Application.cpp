#include "Application.h"

#include "Events/EventVisitor.h"
#include "FileSystem/FileSystem.h"
#include "LayerStack.h"
#include "Mesh.h"
#include "Renderer.h"
#include "KeyCodes.h"
#include "Camera.h"

namespace Fuego
{
class Application::ApplicationImpl
{
    friend class Application;
    std::unique_ptr<Window> m_Window;
    std::unique_ptr<EventQueue> m_EventQueue;
    std::unique_ptr<Renderer::Renderer> _renderer;
    std::unique_ptr<Fuego::FS::FileSystem> _fs;

    bool m_Running;
    LayerStack m_LayerStack;
    static Application* m_Instance;
};
Application* Application::ApplicationImpl::m_Instance = nullptr;

Application::Application()
    : d(new ApplicationImpl())
{
    ApplicationImpl::m_Instance = this;
    d->_fs = std::unique_ptr<Fuego::FS::FileSystem>(new Fuego::FS::FileSystem());
    d->m_EventQueue = EventQueue::CreateEventQueue();
    d->m_Window = Window::CreateAppWindow(WindowProps(), *d->m_EventQueue);

    d->_renderer.reset(new Renderer::Renderer());
    d->m_Running = true;
}

Renderer::Renderer& Application::Renderer()
{
    return *d->_renderer.get();
}

Application::~Application()
{
    delete d;
}

void Application::PushLayer(Layer* layer)
{
    d->m_LayerStack.PushLayer(layer);
    layer->OnAttach();
}

void Application::PushOverlay(Layer* overlay)
{
    d->m_LayerStack.PushOverlay(overlay);
}

void Application::OnEvent(EventVariant& event)
{
    // clang-format off
    auto ApplicationEventVisitor = EventVisitor{[this](WindowCloseEvent&    ev) { OnWindowClose(ev); },
                                                [this](WindowResizeEvent&   ev) { OnWindowResize(ev); },
                                                [this](AppRenderEvent&      ev) { OnRenderEvent(ev); },
                                                [this](WindowStartResizeEvent&    ev) { OnStartResizeWindow(ev); },
                                                [this](WindowEndResizeEvent&    ev) { OnEndResizeWindow(ev); },
                                                [this](WindowValidateEvent&    ev) { OnValidateWindow(ev); },
                                                [this](MouseMovedEvent&    ev) { OnMouseMoveEvent(ev); },
                                                [this](KeyPressedEvent&    ev) { OnKeyPressEvent(ev); },
                                                [](Event&){}};
    // clang-format on

    std::visit(ApplicationEventVisitor, event);

    for (auto it = d->m_LayerStack.end(); it != d->m_LayerStack.begin();)
    {
        (*--it)->OnEvent(event);

        auto HandledEventVisitor = EventVisitor{[](const Event& ev) -> bool
                                                {
                                                    // FU_CORE_TRACE("{0}", ev.ToString());
                                                    return ev.GetHandled();
                                                }};

        if (std::visit(HandledEventVisitor, event))
        {
            break;
        }
    }
}

bool Application::OnWindowClose(WindowCloseEvent& event)
{
    d->m_Running = false;
    event.SetHandled();
    return true;
}

bool Application::OnWindowResize(WindowResizeEvent& event)
{
    d->_renderer->ChangeViewport(event.GetX(), event.GetY(), event.GetWidth(), event.GetHeight());
    event.SetHandled();
    return true;
}

bool Application::OnStartResizeWindow(WindowStartResizeEvent& event)
{
    event.SetHandled();
    return true;
}

bool Application::OnEndResizeWindow(WindowEndResizeEvent& event)
{
    event.SetHandled();
    return true;
}

bool Application::OnValidateWindow(WindowValidateEvent& event)
{
    d->_renderer->ValidateWindow();
    d->m_Window->SetPainted();
    event.SetHandled();
    return true;
}

bool Application::OnKeyPressEvent(KeyPressedEvent& event)
{
    KeyEvent& e = (KeyEvent&)event;
    KeyCode crossplatform_key = e.GetKeyCode();
    
    switch (crossplatform_key)
    {
    case Key::D1:
        d->_renderer->ToggleWireFrame();
        break;

    }
    event.SetHandled();
    return true;
}

bool Application::OnRenderEvent(AppRenderEvent& event)
{
    d->_renderer->ShowWireFrame();
    //d->_renderer->Clear();
    // d->_renderer->DrawMesh(mesh, sizeof(mesh) / sizeof(float), indices, sizeof(indices) / sizeof(unsigned int));
    //d->_renderer->DrawMesh(data, model->GetVertexCount());
    d->_renderer->Present();

    //event.SetHandled();
    UNUSED(event);
    return true;
}

bool Application::OnMouseMoveEvent(MouseMovedEvent& event)
{
    UNUSED(event);
    return true;
}

Application& Application::Get()
{
    return *Application::ApplicationImpl::m_Instance;
}
Fuego::FS::FileSystem& Application::FileSystem()
{
    return *d->_fs.get();
}

Window& Application::GetWindow()
{
    return *d->m_Window;
}

void Application::Run()
{
    while (d->m_Running)
    {
        d->m_EventQueue->Update();
        d->m_Window->Update();
        Fuego::Renderer::Camera::GetActiveCamera()->Update();

        for (auto layer : d->m_LayerStack)
        {
            layer->OnUpdate();
        }

        while (!d->m_EventQueue->Empty())
        {
            auto ev = d->m_EventQueue->Front();
            OnEvent(*ev);
            d->m_EventQueue->Pop();
        }
    }
}


}  // namespace Fuego
