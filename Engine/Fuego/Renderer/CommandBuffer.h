#pragma once

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

    virtual void BindRenderTarget(std::unique_ptr<Texture> texture) = 0;
    virtual void BindVertexShader(std::unique_ptr<Shader> vertexShader) = 0;
    virtual void BindPixelShader(std::unique_ptr<Shader> pixelShader) = 0;
    virtual void BindDescriptorSet(std::unique_ptr<DescriptorBuffer> descriptorSet, int setIndex) = 0;
    virtual void BindVertexBuffer(std::unique_ptr<Buffer> vertexBuffer) = 0;
    virtual void Draw(uint32_t vertexCount) = 0;
};

}  // namespace Fuego::Renderer
