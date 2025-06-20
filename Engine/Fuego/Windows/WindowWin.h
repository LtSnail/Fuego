#pragma once

#include "EventQueueWin.h"
#include "Input.h"
#include "Window.h"

namespace Fuego
{
class SurfaceWindows;

class WindowWin final : public Window
{
public:
    friend class Application;

    WindowWin(const WindowProps& props, EventQueue& eventQueue);

    virtual void OnUpdate(float dlTime) override;
    virtual void OnPostUpdate(float dlTime) override;
    virtual void OnFixedUpdate() override;

    inline virtual unsigned int GetWidth() const override
    {
        return _props.Width;
    }
    inline virtual unsigned int GetHeight() const override
    {
        return _props.Height;
    }

    virtual const void* GetNativeHandle() const override;

    LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    Input::KeyState GetKeyState(KeyCode keyCode) const;
    Input::MouseState GetMouseState(MouseCode mouseCode) const;
    void GetMousePos(float& xPos, float& yPos) const;
    std::pair<float, float> GetMouseWheelScrollData() const;
    virtual inline bool HasMouseMoved(float x, float y) const override
    {
        return !(_cursorPos.x == x && _cursorPos.y == y);
    }

    inline virtual void SwitchInteractionMode() override
    {
        interaction_mode = interaction_mode == InteractionMode::GAMING ? InteractionMode::EDITOR : InteractionMode::GAMING;
    }
    inline virtual InteractionMode GetInteractionMode() const
    {
        return interaction_mode;
    }

    virtual inline bool IsResizing() const override
    {
        return isResizing;
    }
    inline virtual glm::vec2 GetMouseDir() const override
    {
        return _mouseDir;
    }

    inline virtual bool IsActive() const override
    {
        return is_in_focus;
    }

    virtual void SetTitle(std::string title) override;

private:
    float _currentWidth, _currentHeigth;
    int window_center_x, window_center_y, _xPos, _yPos;

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

    bool is_first_launch, isResizing, isPainted, is_in_focus;

    virtual inline void SetPainted() override
    {
        isPainted = true;
    }

    virtual void SetMousePos(float x, float y) override;
    virtual void SetMouseWheelScrollData(float x, float y) override;

    glm::vec2 _mouseDir;
    Input::MouseInfo _lastMouse;
    Input::KeyState pressed_keys[256];
    glm::vec2 _cursorPos;
    glm::vec2 _prevCursorPos;

    std::pair<float, float> mouse_wheel_data;

    InteractionMode interaction_mode;

protected:
    virtual void SetWindowMode(WPARAM mode);
};

}  // namespace Fuego
