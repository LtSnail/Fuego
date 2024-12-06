#pragma once

#if defined(FUEGO_PLATFORM_MACOS)
int FuegoMain(int argc, const char* argv[]);
#elif (FUEGO_PLATFORM_WIN)
int APIENTRY FuegoMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow);
#endif
