#pragma once

#include "Window.h"

namespace Fuego
{
    LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	class WindowWin : public Window
	{
	public:
        WindowWin(const WindowProps& props, EventQueue& eventQueue);

        virtual void Update() override;

        inline virtual unsigned int GetWidth() const override { return m_Props.Width; }
        inline virtual unsigned int GetHeight() const override { return m_Props.Height; }

        virtual void SetVSync(bool enabled) override;
        virtual bool IsVSync() const override;
       
    private:
        void Init(const WindowProps& props, EventQueue& eventQueue);
        void Shutdown();

        // Window handle
        HANDLE m_Window;
        HINSTANCE m_HInstance;  // Relates to the Application
        HWND m_Hwnd;            // Relates to Actual Window instance
        WindowProps m_Props;

        static const TCHAR* APP_WINDOW_CLASS_NAME;
	};
}
