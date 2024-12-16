#include <Fuego/ShaderTranslator.hpp>
#include <cstdio>
#include <cstring>

namespace Fst = Fuego::ShaderTranslator;

int main()
{
    const char* sourceData = "float4 main() : SV_Target0 { return 0.xxxx; }";

    Fst::Arguments arguments = {
        .sourceCodeSize = strlen(sourceData),
        .sourceCode = sourceData,
        .targetStage = Fst::TargetStage::Fragment,
        .targetLanguage = Fst::TargetLanguage::Dxil,
    };

    Fst::Blob blobText;
    Fst::Result result = Fst::Compile(&arguments, nullptr, &blobText);

    if (blobText && Fst::GetBlobSize(blobText))
    {
        fwrite(Fst::GetBlobData(blobText), 1, Fst::GetBlobSize(blobText), stderr);
    }

    Fst::DeleteBlob(&blobText);

    return result == Fst::Result::Ok ? 0 : 1;
}
