#include "Metal/CommandBufferMetal.h"
#include "Metal/ShaderMetal.h"

namespace Fuego::Renderer
{

CommandBufferMetal::CommandBufferMetal()
{
    _renderPipelineDescriptor = MTL::RenderPipelineDescriptor::alloc()->init();
    _renderPipelineDescriptor->setLabel(NS::String::string("Default Rendering Pipeline", NS::ASCIIStringEncoding));
}

void CommandBufferMetal::BindRenderTarget(std::unique_ptr<Texture> texture)
{
}

void CommandBufferMetal::BindVertexShader(std::unique_ptr<Shader> vertexShader)
{
    const auto* metalShader = static_cast<ShaderMetal*>(vertexShader.get());
    _renderPipelineDescriptor->setVertexFunction(metalShader->_function);
    FU_CORE_ASSERT(_renderPipelineDescriptor, "Failed to create Render Pipeline Description");
}

void CommandBufferMetal::BindPixelShader(std::unique_ptr<Shader> pixelShader)
{
    const auto* metalShader = static_cast<ShaderMetal*>(pixelShader.get());
    _renderPipelineDescriptor->setFragmentFunction(metalShader->_function);
    FU_CORE_ASSERT(_renderPipelineDescriptor, "Failed to create Render Pipeline Description");
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
