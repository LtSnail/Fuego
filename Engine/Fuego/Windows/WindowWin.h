#pragma once

#include "EventQueueWin.h"
#include "OpenGL/BufferOpenGL.h"
#include "Window.h"
#include <OpenGL/SurfaceWindows.h>

namespace Fuego
{
#define LAST_CODE UINT16_MAX

LRESULT CALLBACK WindowProcStatic(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class SurfaceWindows;

class WindowWin : public Window
{
public:
    WindowWin(const WindowProps& props, EventQueue& eventQueue);

    virtual void Update() override;

    inline virtual unsigned int GetWidth() const override
    {
        return m_Props.Width;
    }
    inline virtual unsigned int GetHeight() const override
    {
        return m_Props.Height;
    }

    virtual void SetVSync(bool enabled) override;
    virtual bool IsVSync() const override;

    virtual const Renderer::Surface* GetSurface() const override;

    LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
    static std::unordered_map<const HWND*, WindowWin*> hwndMap;

    Input::KeyState GetKeyState(KeyCode keyCode) const;
    Input::MouseState GetMouseState(MouseCode mouseCode) const;
    void GetCursorPos(OUT float& xPos, OUT float& yPos) const;

private:
    void Shutdown();
    static LRESULT CALLBACK WindowProcStatic(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    EventQueueWin* _eventQueue;
    Input::KeyInfo _lastKey;
    Input::MouseInfo _lastMouse;
    Input::CursorPos _cursorPos;

    // Window handle
    HANDLE m_Window;
    HINSTANCE m_HInstance;  // Relates to the Application
    WindowProps m_Props;
    Renderer::SurfaceWindows* _surface;
   
    // Threads
    HANDLE _winThread;
    LPDWORD _winThreadID;
    HANDLE _onThreadCreated;
    static DWORD WinThreadMain(_In_ LPVOID lpParameter);
};

}  // namespace Fuego
