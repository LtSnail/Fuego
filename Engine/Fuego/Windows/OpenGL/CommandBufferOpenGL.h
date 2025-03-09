#pragma once

#include "Renderer/CommandBuffer.h"

namespace Fuego::Renderer
{
class Surface;
class VertexLayout;

class CommandBufferOpenGL final : public CommandBuffer
{
public:
    virtual ~CommandBufferOpenGL() override;
    virtual void BeginRecording() override;
    virtual void EndRecording() override;
    virtual void Submit() override;
    virtual void BindRenderTarget(const Surface& texture) override;
    virtual void BindShaderObject(const ShaderObject& obj) override;
    virtual void BindDescriptorSet(const DescriptorBuffer& descriptorSet, int setIndex) override;
    virtual void BindVertexBuffer(const Buffer& vertexBuffer, VertexLayout layout) override;
    virtual void BindIndexBuffer(uint32_t indices[], uint32_t size) override;
    virtual void BindTexture(Texture* texture) override;
    virtual void Draw(uint32_t vertexCount) override;
    virtual void IndexedDraw(uint32_t vertexCount) override;
    virtual void Clear() override;

private:
    uint32_t _vao;
    uint32_t _ebo;
    uint32_t _mainVsShader;
    uint32_t _pixelShader;
    bool _isLinked;
    bool _isDataAllocated;

    uint32_t _texture;

    friend class CommandPoolOpenGL;
    bool _isFree;
    CommandBufferOpenGL();
};
}  // namespace Fuego::Renderer
