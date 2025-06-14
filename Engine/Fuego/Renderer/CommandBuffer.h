#pragma once
#include <span>

#include "Buffer.h"

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
enum RenderStage;

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
    virtual void BindShaderObject(std::shared_ptr<Fuego::Graphics::ShaderObject> shader) = 0;
    virtual void BindDescriptorSet(const DescriptorBuffer& descriptorSet, int setIndex) = 0;

    virtual void BindVertexBuffer(std::unique_ptr<Fuego::Graphics::Buffer> vertexBuffer, VertexLayout layout) = 0;
    virtual void UpdateSubData(const void* data, size_t size_bytes, size_t offset) = 0;

    virtual void BindIndexBuffer(RenderStage stage, const uint32_t indices[], uint32_t size_bytes) = 0;
    virtual void BindTexture(Texture* texture) = 0;
    virtual void Draw(uint32_t vertexCount) = 0;
    virtual void IndexedDraw(uint32_t vertexCount, const void* indices_ptr_offset) = 0;
    virtual void Clear() = 0;

    virtual void PushDebugGroup(uint32_t id, const char* message) = 0;
    virtual void PopDebugGroup() = 0;
    virtual void SetLabel(ObjectLabel id, uint32_t name, const char* message) = 0;

    virtual Fuego::Graphics::ShaderObject* ShaderObject() const { return shader_object.get(); }

   protected:
    CommandBuffer() : push_debug_group_commands(0) {};

    uint16_t push_debug_group_commands;

    std::shared_ptr<Fuego::Graphics::ShaderObject> shader_object;
    std::unique_ptr<Fuego::Graphics::Buffer> buffer;
};

}  // namespace Fuego::Graphics
