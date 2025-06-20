#pragma once

#include "Renderer/CommandBuffer.h"

namespace Fuego::Graphics
{
class Surface;
struct VertexLayout;

class CommandBufferOpenGL final : public CommandBuffer
{
public:
    virtual ~CommandBufferOpenGL() override;
    virtual void BeginRecording() override;
    virtual void EndRecording() override;
    virtual void Submit() override;
    virtual void BindRenderTarget(const Surface& texture) override;
    virtual void BindShaderObject(std::shared_ptr<Fuego::Graphics::ShaderObject> shader) override;
    virtual void BindDescriptorSet(const DescriptorBuffer& descriptorSet, int setIndex) override;

    virtual void BindVertexBuffer(std::unique_ptr<Buffer> vertexBuffer, VertexLayout layout) override;
    virtual void BindIndexBuffer(std::unique_ptr<Buffer> buffer) override;

    virtual uint32_t UpdateBufferSubDataImpl(Buffer::BufferType type, const void* data, size_t size_bytes) override;

    virtual void BindTexture(Texture* texture) override;
    virtual void Draw(uint32_t vertexCount) override;
    virtual void IndexedDraw(uint32_t index_count, size_t index_offset_bytes, uint32_t base_vertex) override;
    virtual void Clear() override;

    virtual void PushDebugGroup(uint32_t id, const char* message) override;
    virtual void PopDebugGroup() override;
    virtual void SetLabel(ObjectLabel id, uint32_t name, const char* message) override;

private:
    int ConvertUsage(RenderStage& stage) const;
    uint32_t _vao;
    uint32_t _mainVsShader;
    uint32_t _pixelShader;
    bool _isLinked;
    bool _isDataAllocated;

    uint32_t _texture;

    friend class DeviceOpenGL;
    bool _isFree;
    CommandBufferOpenGL();
};
}  // namespace Fuego::Graphics
