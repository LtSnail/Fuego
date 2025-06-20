#pragma once

#include "EventQueue.h"
#include "glm/ext.hpp"
#include "glm/glm.hpp"

namespace Fuego
{

enum WindowMode
{
    MINIMIZED = 0,
    MAXIMIZED = 1,
    RESTORED = 2,
};

struct WindowProps
{
    std::string Title = "Fuego Engine";
    long x = 100;
    long y = 100;
    uint32_t Width = 1280;
    uint32_t Height = 720;
    WindowMode mode = RESTORED;

    bool Centered = true;
    bool Resizable = true;
    bool Movable = true;
    bool Closable = true;
    bool Minimizable = true;
    bool Maximizable = true;
    bool CanFullscreen = true;

    uint32_t BackgroundColor = 0xFFFFFFFF;
    bool Transparent = false;
    bool Frame = true;
    bool HasShadow = true;

#ifdef FUEGO_PLATFORM_WIN
    const TCHAR* APP_WINDOW_CLASS_NAME = TEXT("MainAppWindow Engine");
#endif
};

class Surface;

class Window : public IUpdatable
{
    friend class Application;

public:
    enum InteractionMode
    {
        EDITOR = 0,
        GAMING = 1
    };
    virtual ~Window() = default;

    virtual void OnUpdate(float dlTime) = 0;
    virtual void OnPostUpdate(float dlTime) = 0;
    virtual void OnFixedUpdate() = 0;

    virtual unsigned int GetWidth() const = 0;
    virtual unsigned int GetHeight() const = 0;

    virtual bool IsActive() const = 0;

    virtual const void* GetNativeHandle() const = 0;

    virtual inline bool IsResizing() const = 0;

    inline virtual glm::vec2 GetMouseDir() const = 0;
    virtual bool HasMouseMoved(float x, float y) const = 0;

    virtual InteractionMode GetInteractionMode() const = 0;
    virtual void SwitchInteractionMode() = 0;

    virtual void SetTitle(std::string title) = 0;

    static std::unique_ptr<Window> CreateAppWindow(const WindowProps& props, EventQueue& eventQueue);

private:
    virtual void SetMousePos(float x, float y) = 0;
    virtual void SetPainted() = 0;
    virtual void SetMouseWheelScrollData(float x, float y) = 0;
};
}  // namespace Fuego
