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

void CommandBufferMetal::BindRenderTarget(const Texture& texture)
{
    const auto* metalTexture = static_cast<const TextureMetal*>(&texture);
    MTL::PixelFormat pixelFormat = (MTL::PixelFormat)metalTexture->GetTextureFormat();
    _renderPipelineDescriptor->colorAttachments()->object(0)->setPixelFormat(pixelFormat);
    FU_CORE_ASSERT(_renderPipelineDescriptor, "Failed to bind Render Target");
}

void CommandBufferMetal::BindVertexShader(const Shader& vertexShader)
{
    const auto* metalShader = static_cast<const ShaderMetal*>(&vertexShader);
    _renderPipelineDescriptor->setVertexFunction(metalShader->_function);
    FU_CORE_ASSERT(_renderPipelineDescriptor, "Failed to bind Vertex Shader");
}

void CommandBufferMetal::BindPixelShader(const Shader& pixelShader)
{
    const auto* metalShader = static_cast<const ShaderMetal*>(&pixelShader);
    _renderPipelineDescriptor->setFragmentFunction(metalShader->_function);
    FU_CORE_ASSERT(_renderPipelineDescriptor, "Failed to bind Pixel Shader");
}

void CommandBufferMetal::BindDescriptorSet(const DescriptorBuffer& descriptorSet, int setIndex)
{
}

void CommandBufferMetal::BindVertexBuffer(const Buffer& vertexBuffer)
{
}

void CommandBufferMetal::Draw(uint32_t vertexCount)
{
}

}  // namespace Fuego::Renderer
