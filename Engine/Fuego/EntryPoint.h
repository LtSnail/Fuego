#pragma once

// Compiled in a client application
#if defined(FUEGO_PLATFORM_MACOS)

@interface FuegoApplication : NSApplication

@property Fuego::Application* application;

- (void)run;

@end

@implementation FuegoApplication
;

- (void)run
{
    [[NSNotificationCenter defaultCenter] postNotificationName:NSApplicationWillFinishLaunchingNotification object:NSApp];
    [[NSNotificationCenter defaultCenter] postNotificationName:NSApplicationDidFinishLaunchingNotification object:NSApp];

    _application->Run();
}

@end

int main(int argc, const char* argv[])
{
    Fuego::Log::Init();

    FuegoApplication* applicationObject = [FuegoApplication alloc];

    Fuego::Application* app = Fuego::CreateApplication();
    applicationObject.application = app;

    if ([applicationObject respondsToSelector:@selector(run)])
    {
        [applicationObject performSelectorOnMainThread:@selector(run) withObject:nil waitUntilDone:YES];
    }

    delete app;

    return 0;
}
#elif (FUEGO_PLATFORM_WIN)
#include "fupch.h"

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
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

#endif
