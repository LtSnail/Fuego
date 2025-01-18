#pragma once

#include "EventQueueWin.h"
#include "Input.h"
#include "Window.h"


namespace Fuego
{
class SurfaceWindows;

class WindowWin : public Window
{
public:
    WindowWin(const WindowProps& props, EventQueue& eventQueue);

    virtual void Update() override;

    inline virtual unsigned int GetWidth() const override
    {
        return _props.Width;
    }
    inline virtual unsigned int GetHeight() const override
    {
        return _props.Height;
    }

    virtual void SetVSync(bool enabled) override;
    virtual bool IsVSync() const override;

    virtual const void* GetNativeHandle() const override;

    LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    Input::KeyState GetKeyState(KeyCode keyCode) const;
    Input::MouseState GetMouseState(MouseCode mouseCode) const;
    void GetMousePos(OUT float& xPos, OUT float& yPos) const;


    virtual inline bool IsResizing() const
    {
        return isResizing;
    }
    inline virtual glm::vec2 GetMouseDir() const override
    {
        return _mouseDir;
    }

private:
    float _xPos, _yPos, _currentWidth, _currentHeigth;

    static DWORD WinThreadMain(_In_ LPVOID lpParameter);
    static LRESULT CALLBACK WindowProcStatic(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static void InitOpenGLExtensions();

    EventQueueWin* _eventQueue;


    // Window handle
    HWND _hwnd;
    HINSTANCE _hinstance;  // Relates to the Application
    WindowProps _props;

    // Threads
    HANDLE _winThread;
    LPDWORD _winThreadID;
    HANDLE _onThreadCreated;

    bool isResizing;
    bool isPainted;

    friend class Application;
    virtual inline void SetPainted() override
    {
        isPainted = true;
    }

    virtual void SetMousePos(float x, float y) override;

    glm::vec2 _mouseDir;
    Input::KeyInfo _lastKey;
    Input::MouseInfo _lastMouse;
    glm::vec2 _cursorPos;
    glm::vec2 _prevCursorPos;


protected:
    virtual void SetWindowMode(WPARAM mode);
};

}  // namespace Fuego
