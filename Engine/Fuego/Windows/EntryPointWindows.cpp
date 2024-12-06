#include <EntryPoint.h>
#include <Application.h>

int APIENTRY FuegoMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
    UNUSED(hInst);
    UNUSED(hInstPrev);
    UNUSED(cmdline);
    UNUSED(cmdshow);

    AllocConsole();
    FILE* stream;
    freopen_s(&stream, "CONOUT$", "w", stdout);
    freopen_s(&stream, "CONOUT$", "w", stderr);

    Fuego::Log::Init();

    Fuego::Application* app = Fuego::CreateApplication();
    app->Run();

    FreeConsole();
    delete app;
}
