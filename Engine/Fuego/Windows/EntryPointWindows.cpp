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

    Fuego::Application* app = Fuego::CreateApplication();
    app->Run();

    FreeConsole();
    delete app;

    return 0;
}
