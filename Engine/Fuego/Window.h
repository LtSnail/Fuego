#pragma once

#include "EventQueue.h"
#include "Events/Event.h"
#include "Input.h"
#include "Renderer/Surface.h"
#include "fupch.h"

namespace Fuego
{
struct WindowProps
{
    std::string Title = "Fuego Engine";
    long x = 100;
    long y = 100;
    unsigned int Width = 1280;
    unsigned int Height = 720;

    bool Centered = true;
    bool Resizable = true;
    bool Movable = true;
    bool Closable = true;
    bool Minimizable = true;
    bool Maximizable = true;
    bool CanFullscreen = true;

    unsigned BackgroundColor = 0xFFFFFFFF;
    bool Transparent = false;
    bool Frame = true;
    bool HasShadow = true;

#ifdef FUEGO_PLATFORM_WIN
    const TCHAR* APP_WINDOW_CLASS_NAME = TEXT("MainAppWindow Engine");
#endif
};

class Surface;

class Window
{
public:
    virtual ~Window() = default;

    virtual void Update() = 0;

    virtual unsigned int GetWidth() const = 0;
    virtual unsigned int GetHeight() const = 0;

    virtual void SetVSync(bool enabled) = 0;
    virtual bool IsVSync() const = 0;

    virtual const Renderer::Surface* GetSurface() const = 0;

    static std::unique_ptr<Window> CreateAppWindow(const WindowProps& props, EventQueue& eventQueue);
};
}  // namespace Fuego
