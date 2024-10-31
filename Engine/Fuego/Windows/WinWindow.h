#include <Windows.h>
#include "Window.h"

namespace Fuego
{
	class WindowsWindow : public Window
	{
	public:
        WindowsWindow(const WindowProps& props, EventQueue& eventQueue);
        ~WindowsWindow();

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
