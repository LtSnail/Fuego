#pragma once

#include "Renderer/CommandBuffer.h"

namespace Fuego::Renderer
{
class Surface;

class CommandBufferOpenGL : public CommandBuffer
{
public:
    CommandBufferOpenGL();
    virtual ~CommandBufferOpenGL() override;

    virtual void BindRenderTarget(const Surface& texture) override;
    virtual void BindVertexShader(const Shader& vertexShader) override;
    virtual void BindPixelShader(const Shader& pixelShader) override;
    virtual void BindDescriptorSet(const DescriptorBuffer& descriptorSet, int setIndex) override;
    virtual void BindVertexBuffer(const Buffer& vertexBuffer) override;
    virtual void Draw(uint32_t vertexCount) override;

protected:
private:
    uint32_t _programID;
};
}  // namespace Fuego::Renderer
