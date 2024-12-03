#pragma once

#include "Renderer/CommandBuffer.h"

namespace Fuego::Renderer
{
class CommandBufferOpenGL : public CommandBuffer
{
public:
    virtual ~CommandBufferOpenGL() override;

    virtual void BindRenderTarget(std::unique_ptr<Texture> texture) override;
    virtual void BindVertexShader(std::unique_ptr<Shader> vertexShader) override;
    virtual void BindPixelShader(std::unique_ptr<Shader> pixelShader) override;
    virtual void BindDescriptorSet(std::unique_ptr<DescriptorBuffer> descriptorSet, int setIndex) override;
    virtual void BindVertexBuffer(std::unique_ptr<Buffer> vertexBuffer) override;
    virtual void Draw(uint32_t vertexCount) override;

protected:
    CommandBufferOpenGL();
};
}  // namespace Fuego::Renderer
