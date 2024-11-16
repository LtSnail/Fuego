#pragma once

#include "EventQueueWin.h"
#include "InputWin.h"
#include "Window.h"

namespace Fuego
{
#define LAST_CODE UINT16_MAX

LRESULT CALLBACK WindowProcStatic(HWND hWnd, UINT uMsg, WPARAM wParam,
                                  LPARAM lParam);

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

    inline virtual void* GetNativeWindow() const { return m_Hwnd; }

    LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
    static std::unordered_map<HWND, WindowWin*> hwndMap;

    enum KeyState
    {
        KEY_NONE = 0,
        KEY_PRESSED = 1,
        KEY_REPEAT = 2,
    };

    struct KeyInfo
    {
        KeyState state;
        uint16_t keyCode;
    };

    enum MouseState
    {
        MOUSE_NONE,
        MOUSE_LPRESSED,
        MOUSE_RPRESSED,
        MOUSE_MPRESSED,
        MOUSE_SCROLL
    };

    struct MouseInfo
    {
        MouseState state;
        uint16_t mouseCode;
    };

    struct CursorPos
    {
        float x;
        float y;
    };

    KeyState GetKeyState(uint16_t keyCode) const;
    MouseState GetMouseState(uint16_t mouseCode) const;
    void GetCursorPos(OUT float& xPos, OUT float& yPos) const;

   private:
    void Shutdown();
    static LRESULT CALLBACK WindowProcStatic(HWND hWnd, UINT uMsg,
                                             WPARAM wParam, LPARAM lParam);

    EventQueueWin* m_EventQueue;
    KeyInfo _lastKey;
    MouseInfo _lastMouse;
    CursorPos _cursorPos;

    // Window handle
    HANDLE m_Window;
    HINSTANCE m_HInstance;  // Relates to the Application
    HWND m_Hwnd;            // Relates to Actual Window instance
    WindowProps m_Props;
};
}  // namespace Fuego
