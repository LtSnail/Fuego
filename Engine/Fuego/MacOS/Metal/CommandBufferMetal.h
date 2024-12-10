#pragma once

#include "Renderer/CommandBuffer.h"

#include <Metal/Metal.hpp>

namespace Fuego::Renderer
{

class CommandBufferMetal : public CommandBuffer
{
public:
    CommandBufferMetal();
    ~CommandBufferMetal();

    virtual void BindRenderTarget(const Texture& texture) override;
    virtual void BindVertexShader(const Shader& vertexShader) override;
    virtual void BindPixelShader(const Shader& pixelShader) override;
    virtual void BindDescriptorSet(const DescriptorBuffer& descriptorSet, int setIndex) override;
    virtual void BindVertexBuffer(const Buffer& vertexBuffer) override;
    virtual void Draw(uint32_t vertexCount) override;
    
private:
    MTL::RenderPipelineDescriptor* _renderPipelineDescriptor;
};

}  // namespace Fuego::Renderer
