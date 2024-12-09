#include "Application.h"

#include "Events/EventVisitor.h"
#include "Input.h"
#include "Renderer/Buffer.h"
#include "Renderer/CommandBuffer.h"
#include "Renderer/CommandPool.h"
#include "Renderer/CommandQueue.h"
#include "Renderer/Device.h"
#include "Renderer/Shader.h"
#include "Renderer/Swapchain.h"

namespace Fuego
{
class Application::ApplicationImpl
{
    friend class Application;

    std::unique_ptr<Window> m_Window;
    std::unique_ptr<EventQueue> m_EventQueue;
    std::unique_ptr<Renderer::Device> _device;
    std::unique_ptr<Renderer::CommandQueue> _commandQueue;
    std::unique_ptr<Renderer::CommandPool> _commandPool;
    std::unique_ptr<Renderer::Swapchain> _swapchain;
    std::unique_ptr<Renderer::Shader> _vertexShader;
    std::unique_ptr<Renderer::Shader> _pixelShader;
    bool m_Running;
    LayerStack m_LayerStack;
    static Application* m_Instance;
};
Application* Application::ApplicationImpl::m_Instance = nullptr;

Application::Application()
    : d(new ApplicationImpl())
{
    ApplicationImpl::m_Instance = this;
    d->m_EventQueue = EventQueue::CreateEventQueue();
    d->m_Window = Window::CreateAppWindow(WindowProps(), *d->m_EventQueue);
    d->m_Running = true;

    // Temporarily here.
    d->_device = Renderer::Device::CreateDevice(*d->m_Window->GetSurface());
    d->_commandQueue = d->_device->CreateQueue();
    d->_commandPool = d->_device->CreateCommandPool(*d->_commandQueue);
    d->_swapchain = d->_device->CreateSwapchain(*d->m_Window->GetSurface());

    d->_vertexShader = d->_device->CreateShader("vs_triangle");
    d->_pixelShader = d->_device->CreateShader("ps_triangle");
}

Application::~Application()
{
    delete d;
}

void Application::PushLayer(Layer* layer)
{
    d->m_LayerStack.PushLayer(layer);
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
                                                [](Event&){}};
    // clang-format on

    std::visit(ApplicationEventVisitor, event);

    for (auto it = d->m_LayerStack.end(); it != d->m_LayerStack.begin();)
    {
        (*--it)->OnEvent(event);

        auto HandledEventVisitor = EventVisitor{[](const Event& ev) -> bool
                                                {
                                                    FU_CORE_TRACE("{0}", ev.ToString());
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
    UNUSED(event);

    // TODO: Recreate swapchain?

    return true;
}

bool Application::OnRenderEvent(AppRenderEvent& event)
{
    static constexpr float vertices[] = {
        // Positions        // Colors
        0.0f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f,  // Top vertex (Red)
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // Bottom-left vertex (Green)
        0.5f,  -0.5f, 0.0f, 0.0f, 0.0f, 1.0f   // Bottom-right vertex (Blue)
    };

    static std::unique_ptr<Renderer::Buffer> vertexBuffer = d->_device->CreateBuffer(sizeof(vertices), 0);
    vertexBuffer->BindData<float>(std::span(vertices));

    std::unique_ptr<Renderer::CommandBuffer> cmd = d->_commandPool->CreateCommandBuffer();

    cmd->BindVertexBuffer(*vertexBuffer);
    cmd->BindVertexShader(*d->_vertexShader);
    cmd->BindPixelShader(*d->_pixelShader);
    cmd->Draw(3);

    d->_swapchain->Present();

    event.SetHandled();
    return true;
}

Application& Application::Get()
{
    return *Application::ApplicationImpl::m_Instance;
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
