#pragma once

#include "EventQueueWin.h"
#include "Window.h"

namespace Fuego
{
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

    LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
    static std::unordered_map<HWND, WindowWin*> hwndMap;

   private:
    void Init(const WindowProps& props, EventQueue& eventQueue);
    void Shutdown();
    static LRESULT CALLBACK WindowProcStatic(HWND hWnd, UINT uMsg,
                                             WPARAM wParam, LPARAM lParam);

    EventQueue& m_EventQueue;

    // Window handle
    HANDLE m_Window;
    HINSTANCE m_HInstance;  // Relates to the Application
    HWND m_Hwnd;            // Relates to Actual Window instance
    WindowProps m_Props;
};
}  // namespace Fuego
