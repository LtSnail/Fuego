#pragma once

namespace Fuego::Graphics
{
class Surface;
class Shader;
class DescriptorBuffer;
class Buffer;
class ShaderObject;
class Texture;
struct VertexLayout;
class Device;

class CommandBuffer
{
public:
    enum ObjectLabel
    {
        LABEL_BUFFER = 0,
        LABEL_SHADER = 1,
        LABEL_TEXTURE = 2
    };
    static std::unique_ptr<CommandBuffer> CreateCommandBuffer();

    virtual ~CommandBuffer() = default;

    virtual void BeginRecording() = 0;
    virtual void EndRecording() = 0;
    virtual void Submit() = 0;
    virtual void BindRenderTarget(const Surface& texture) = 0;
    virtual void BindShaderObject(const ShaderObject& obj) = 0;
    virtual void BindDescriptorSet(const DescriptorBuffer& descriptorSet, int setIndex) = 0;
    virtual void BindVertexBuffer(const Buffer& vertexBuffer, VertexLayout layout) = 0;
    virtual void BindIndexBuffer(const uint32_t indices[], uint32_t size) = 0;
    virtual void BindTexture(Texture* texture) = 0;
    virtual void Draw(uint32_t vertexCount) = 0;
    virtual void IndexedDraw(uint32_t vertexCount, const void* indices_ptr_offset) = 0;
    virtual void Clear() = 0;

    virtual void PushDebugGroup(uint32_t id, const char* message) = 0;
    virtual void PopDebugGroup() = 0;
    virtual void SetLabel(ObjectLabel id, uint32_t name, const char* message) = 0;

protected:
    CommandBuffer()
        : push_debug_group_commands(0)
    {
    }

protected:
    uint16_t push_debug_group_commands;
};

}  // namespace Fuego::Graphics
