#include <Fuego/ShaderTranslator.hpp>
#ifdef _WIN32
#include <Windows.h>
#include <atlbase.h>
#else
#define __EMULATE_UUID 1
#endif
#include <dxc/Support/Unicode.h>
#include <dxc/dxcapi.h>

namespace Fst = Fuego::ShaderTranslator;

static const char* FstTargetToHlslText(Fst::TargetStage stage)
{
    switch (stage)
    {
    case Fst::TargetStage::Vertex:
        return "vs_6_0";
    case Fst::TargetStage::Fragment:
        return "ps_6_0";
    case Fst::TargetStage::Compute:
        return "cs_6_0";
    }

    return nullptr;
}

static Fst::Result FstHresultToResult(HRESULT hresult)
{
    switch (hresult)
    {
    case S_OK:
        return Fst::Result::Ok;
    }

    return Fst::Result::ErrorUnknown;
}

static bool FstHresultCheck(HRESULT hresult, Fst::Result* outResult)
{
    *outResult = FstHresultToResult(hresult);
    return *outResult == Fst::Result::Ok;
}

Fst::Result Fst::Compile(const Arguments* arguments, Blob* outCompiled, Blob* outText)
{
    if (outCompiled)
        *outCompiled = nullptr;
    if (outText)
        *outText = nullptr;

    const char* sourcePath = arguments->sourcePath;
    const char* entryPoint = arguments->entryPoint ? arguments->entryPoint : "main";
    const char* hlslTarget = FstTargetToHlslText(arguments->targetStage);
    size_t sourceCodeSize = arguments->sourceCodeSize;
    const void* sourceCode = arguments->sourceCode;

    if (!hlslTarget)
    {
        return Result::ErrorInvalidArgument;
    }

    if (!sourceCodeSize || !sourceCode)
    {
        // TODO read file
        return Result::ErrorNoSuchFile;
    }

    Fst::Result result = Fst::Result::Ok;

    CComPtr<IDxcUtils> utils;
    if (!FstHresultCheck(DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&utils)), &result))
        return result;

    CComPtr<IDxcCompilerArgs> args;
    if (!FstHresultCheck(utils->BuildArguments(Unicode::UTF8ToWideStringOrThrow(sourcePath).c_str(), Unicode::UTF8ToWideStringOrThrow(entryPoint).c_str(),
                                               Unicode::UTF8ToWideStringOrThrow(hlslTarget).c_str(), nullptr, 0, nullptr, 0, &args),
                         &result))
        return result;

    CComPtr<IDxcCompiler3> compiler;
    if (!FstHresultCheck(DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&compiler)), &result))
        return result;

    DxcBuffer sourceBuffer = {
        .Ptr = sourceCode,
        .Size = sourceCodeSize,
        .Encoding = DXC_CP_UTF8,
    };

    CComPtr<IDxcResult> compilerResult;
    if (!FstHresultCheck(compiler->Compile(&sourceBuffer, args->GetArguments(), args->GetCount(), nullptr, IID_PPV_ARGS(&compilerResult)), &result) &&
        !compilerResult)
        return result;

    CComPtr<IDxcBlobUtf8> utf8ErrorBlob;
    CComPtr<IDxcBlobEncoding> errorBlob;
    if (outText && FstHresultCheck(compilerResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&errorBlob), nullptr), &result))
    {
        if (!FstHresultCheck(utils->GetBlobAsUtf8(errorBlob, &utf8ErrorBlob), &result))
            return result;
    }

    CComPtr<IDxcBlob> compiledBlob;
    if (!FstHresultCheck(compilerResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&compiledBlob), nullptr), &result))
        return result;

    if (outCompiled)
    {
        *outCompiled = compiledBlob.Detach();
    }

    if (outText)
    {
        *outText = utf8ErrorBlob.Detach();
    }

    HRESULT status;
    if (!FstHresultCheck(compilerResult->GetStatus(&status), &result) || !FstHresultCheck(status, &result))
    {
        return result;
    }


    return Fst::Result::Ok;
}

void* Fst::GetBlobData(Blob blob)
{
    return blob ? static_cast<IDxcBlob*>(blob)->GetBufferPointer() : nullptr;
}

size_t Fst::GetBlobSize(Blob blob)
{
    return blob ? static_cast<IDxcBlob*>(blob)->GetBufferSize() : 0;
}

void Fst::DeleteBlob(Blob* blob)
{
    if (!blob || !*blob)
        return;
    static_cast<IDxcBlob*>(*blob)->Release();
    *blob = nullptr;
}
