#ifdef _WIN32
#include <Windows.h>
#else
#define __EMULATE_UUID 1
#endif
#include <dxc/dxcapi.h>

int main()
{
    IDxcUtils* utils;
    if (DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&utils)) != S_OK)
        return 1;
    utils->Release();
    return 0;
}
