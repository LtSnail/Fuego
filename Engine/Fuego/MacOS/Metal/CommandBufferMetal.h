#pragma once

#include <Metal/Metal.hpp>

#include "Renderer/CommandBuffer.h"

namespace Fuego::Renderer
{

class ShaderMetal;
class BufferMetal;
class SurfaceMetal;

class CommandBufferMetal : public CommandBuffer
{
public:
    CommandBufferMetal(MTL::CommandBuffer* commandBuffer, MTL::Device* device);
    ~CommandBufferMetal();

    virtual void BeginRecording() override
    {
    }
    virtual void EndRecording() override
    {
    }
    virtual void Submit() override
    {
    }

    virtual void BindShaderObject(const ShaderObject& obj) override
    { /* TODO */
    }
    virtual void BindRenderTarget(const Surface& texture) override;

    // TODO: remove
    virtual void BindVertexShader(const Shader& vertexShader);
    virtual void BindPixelShader(const Shader& pixelShader);

    virtual void BindDescriptorSet(const DescriptorBuffer& descriptorSet, int setIndex) override;
    virtual void BindVertexBuffer(const Buffer& vertexBuffer, VertexLayout layout) override;
    virtual void BindIndexBuffer(uint32_t indices[], uint32_t size) override
    {
        UNUSED(indices);
        UNUSED(size);
    }
    virtual void Draw(uint32_t vertexCount) override;
    virtual void IndexedDraw(uint32_t vertexCount) override
    {
        UNUSED(vertexCount);
    }
    virtual void Clear() override
    {
    }

private:
    MTL::CommandBuffer* _commandBuffer;
    MTL::Device* _device;

    const ShaderMetal* _vertexShader;
    const ShaderMetal* _pixelShader;
    const BufferMetal* _buffer;
    const SurfaceMetal* _surface;
};

}  // namespace Fuego::Renderer
