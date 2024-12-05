#include "Metal/CommandBufferMetal.h"
#include "Metal/ShaderMetal.h"
#include "Metal/TextureMetal.h"

namespace Fuego::Renderer
{

CommandBufferMetal::CommandBufferMetal()
{
    _renderPipelineDescriptor = MTL::RenderPipelineDescriptor::alloc()->init();
    _renderPipelineDescriptor->setLabel(NS::String::string("Default Rendering Pipeline", NS::ASCIIStringEncoding));
}

void CommandBufferMetal::BindRenderTarget(std::unique_ptr<Texture> texture)
{
    const auto* metalTexture = static_cast<TextureMetal*>(texture.get());
    MTL::PixelFormat pixelFormat = (MTL::PixelFormat)metalTexture->GetTextureFormat();
    _renderPipelineDescriptor->colorAttachments()->object(0)->setPixelFormat(pixelFormat);
    FU_CORE_ASSERT(_renderPipelineDescriptor, "Failed to bind Render Target");
}

void CommandBufferMetal::BindVertexShader(std::unique_ptr<Shader> vertexShader)
{
    const auto* metalShader = static_cast<ShaderMetal*>(vertexShader.get());
    _renderPipelineDescriptor->setVertexFunction(metalShader->_function);
    FU_CORE_ASSERT(_renderPipelineDescriptor, "Failed to bind Vertex Shader");
}

void CommandBufferMetal::BindPixelShader(std::unique_ptr<Shader> pixelShader)
{
    const auto* metalShader = static_cast<ShaderMetal*>(pixelShader.get());
    _renderPipelineDescriptor->setFragmentFunction(metalShader->_function);
    FU_CORE_ASSERT(_renderPipelineDescriptor, "Failed to bind Pixel Shader");
}

void CommandBufferMetal::BindDescriptorSet(std::unique_ptr<DescriptorBuffer> descriptorSet, int setIndex)
{
}

void CommandBufferMetal::BindVertexBuffer(std::unique_ptr<Buffer> vertexBuffer)
{
}

void CommandBufferMetal::Draw(uint32_t vertexCount)
{
}

}  // namespace Fuego::Renderer
