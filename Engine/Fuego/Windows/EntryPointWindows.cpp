#include <Application.h>
#include <EntryPoint.h>

int APIENTRY FuegoMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
    UNUSED(hInst);
    UNUSED(hInstPrev);
    UNUSED(cmdline);
    UNUSED(cmdshow);

    AllocConsole();
    FILE* stream;
    assert(freopen_s(&stream, "CONOUT$", "w", stdout) == 0);
    assert(freopen_s(&stream, "CONOUT$", "w", stderr) == 0);

    Fuego::Log::Init();

    Fuego::Application::ApplicationBootSettings settings{};
    settings.renderer = Fuego::Application::RendererType::OpenGL;
    settings.vsync = false;
    settings.fixed_dt = 0.025f;
    settings.window_props.x = 5;
    settings.window_props.y = 5;
    settings.window_props.CanFullscreen = true;

    Fuego::Application::instance().Init(settings);
    Fuego::Application::instance().Run();

    FreeConsole();

    return 0;
}
