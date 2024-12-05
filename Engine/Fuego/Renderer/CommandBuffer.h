#pragma once

#include <memory>

namespace Fuego::Renderer
{
class Texture;
class Shader;
class DescriptorBuffer;
class Buffer;

class CommandBuffer
{
public:
    virtual ~CommandBuffer() = default;

    virtual void BindRenderTarget(const Texture& texture) = 0;
    virtual void BindVertexShader(const Shader& vertexShader) = 0;
    virtual void BindPixelShader(const Shader& pixelShader) = 0;
    virtual void BindDescriptorSet(const DescriptorBuffer& descriptorSet, int setIndex) = 0;
    virtual void BindVertexBuffer(const Buffer& vertexBuffer) = 0;
    virtual void Draw(uint32_t vertexCount) = 0;
};

}  // namespace Fuego::Renderer
