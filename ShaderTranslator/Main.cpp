#ifdef _WIN32
#include <Windows.h>
#include <atlbase.h>
#else
#define __EMULATE_UUID 1
#endif
#include <dxc/Support/Unicode.h>
#include <dxc/dxcapi.h>

int main()
{
    const char* sourceName = "source name";
    const char* entryPoint = "main";
    const char* hlslTarget = "ps_6_0";
    const char* sourceData = "float4 main() : SV_Target0 { return 0.xxxx; }";

    CComPtr<IDxcUtils> utils;
    if (DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&utils)) != S_OK)
        return 1;

    CComPtr<IDxcCompilerArgs> args;
    if (utils->BuildArguments(Unicode::UTF8ToWideStringOrThrow(sourceName).c_str(), Unicode::UTF8ToWideStringOrThrow(entryPoint).c_str(),
                              Unicode::UTF8ToWideStringOrThrow(hlslTarget).c_str(), nullptr, 0, nullptr, 0, &args) != S_OK)
        return 1;

    CComPtr<IDxcCompiler3> compiler;
    if (DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&compiler)) != S_OK)
        return 1;

    DxcBuffer sourceBuffer = {
        .Ptr = sourceData,
        .Size = strlen(sourceData),
        .Encoding = DXC_CP_UTF8,
    };

    CComPtr<IDxcResult> result;
    if (compiler->Compile(&sourceBuffer, args->GetArguments(), args->GetCount(), nullptr, IID_PPV_ARGS(&result)) != S_OK && !result)
        return 1;

    CComPtr<IDxcBlobEncoding> errorBlob;
    if (result->GetErrorBuffer(&errorBlob) == S_OK && errorBlob)
    {
        CComPtr<IDxcBlobUtf8> utf8ErrorBlob;
        if (utils->GetBlobAsUtf8(errorBlob, &utf8ErrorBlob) != S_OK)
            return 1;

        fwrite(utf8ErrorBlob->GetBufferPointer(), 1, utf8ErrorBlob->GetBufferSize(), stderr);
    }

    HRESULT status;
    if (result->GetStatus(&status) != S_OK || status != S_OK)
    {
        return 1;
    }

    return 0;
}
