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
    Fuego::Application::instance().Init();
    Fuego::Application::instance().SetVSync(false);
    Fuego::Application::instance().Run();

    FreeConsole();

    return 0;
}
