#pragma once

namespace Fuego::Renderer
{
class Surface;
class Shader;
class DescriptorBuffer;
class Buffer;
class ShaderObject;
class Texture;
struct VertexLayout;

class CommandBuffer
{
public:
    static std::unique_ptr<CommandBuffer> CreateCommandBuffer();

    virtual ~CommandBuffer() = default;

    virtual void BeginRecording() = 0;
    virtual void EndRecording() = 0;
    virtual void Submit() = 0;
    virtual void BindRenderTarget(const Surface& texture) = 0;
    virtual void BindShaderObject(const ShaderObject& obj) = 0;
    virtual void BindDescriptorSet(const DescriptorBuffer& descriptorSet, int setIndex) = 0;
    virtual void BindVertexBuffer(const Buffer& vertexBuffer, VertexLayout layout) = 0;
    virtual void BindIndexBuffer(uint32_t indices[], uint32_t size) = 0;
    virtual void BindTexture(Texture* texture) = 0;
    virtual void Draw(uint32_t vertexCount) = 0;
    virtual void IndexedDraw(uint32_t vertexCount) = 0;
    virtual void Clear() = 0;
};

}  // namespace Fuego::Renderer
