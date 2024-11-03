#pragma once

#include "Window.h"

namespace Fuego
{
	class WinWindow : public Window
	{
	public:
        WinWindow(const WindowProps& props, EventQueue& eventQueue);

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

        WindowProps m_Props;
	};
}
