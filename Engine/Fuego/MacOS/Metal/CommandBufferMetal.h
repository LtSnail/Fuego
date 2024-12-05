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

    virtual void BindRenderTarget(std::unique_ptr<Texture> texture) override;
    virtual void BindVertexShader(std::unique_ptr<Shader> vertexShader) override;
    virtual void BindPixelShader(std::unique_ptr<Shader> pixelShader) override;
    virtual void BindDescriptorSet(std::unique_ptr<DescriptorBuffer> descriptorSet, int setIndex) override;
    virtual void BindVertexBuffer(std::unique_ptr<Buffer> vertexBuffer) override;
    virtual void Draw(uint32_t vertexCount) override;
    
private:
    MTL::RenderPipelineDescriptor* _renderPipelineDescriptor;
};

}  // namespace Fuego::Renderer
