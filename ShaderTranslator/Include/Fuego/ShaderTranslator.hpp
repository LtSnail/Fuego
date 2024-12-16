#pragma once

namespace Fuego::ShaderTranslator
{
enum class TargetLanguage
{
    Dxil,
};

enum class TargetStage
{
    Vertex,
    Fragment,
    Compute,
};

enum class Result
{
    Ok,
    ErrorSourceCode,
    ErrorInvalidArgument,
    ErrorNoSuchFile,
    ErrorUnknown,
};

struct Arguments
{
    const char* sourcePath;
    size_t sourceCodeSize;
    const void* sourceCode;
    const char* entryPoint;
    TargetStage targetStage;
    TargetLanguage targetLanguage;
};

using Blob = void*;

Result Compile(const Arguments*, Blob* outCompiled, Blob* outText);
void* GetBlobData(Blob);
size_t GetBlobSize(Blob);
void DeleteBlob(Blob*);

}  // namespace Fuego::ShaderTranslator
