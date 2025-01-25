#pragma once

namespace Fuego::Renderer
{
class Surface;
class Shader;
class DescriptorBuffer;
class Buffer;

class CommandBuffer
{
public:
    static std::unique_ptr<CommandBuffer> CreateCommandBuffer();

    virtual ~CommandBuffer() = default;

    virtual void BeginRecording() = 0;
    virtual void EndRecording() = 0;
    virtual void Submit() = 0;
    virtual void BindRenderTarget(const Surface& texture) = 0;
    virtual void BindVertexShader(const Shader& vertexShader) = 0;
    virtual void BindPixelShader(const Shader& pixelShader) = 0;
    virtual void BindDescriptorSet(const DescriptorBuffer& descriptorSet, int setIndex) = 0;
    virtual void BindVertexBuffer(const Buffer& vertexBuffer) = 0;
    virtual void BindIndexBuffer(uint32_t indices[], uint32_t size) = 0;
    virtual void BindTexture(unsigned char* data, int w, int h) = 0;
    virtual void Draw(uint32_t vertexCount) = 0;
    virtual void IndexedDraw(uint32_t vertexCount) = 0;
    virtual void Clear() = 0;
};

}  // namespace Fuego::Renderer
