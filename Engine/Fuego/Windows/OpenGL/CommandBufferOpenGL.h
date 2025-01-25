#pragma once

#include "Renderer/CommandBuffer.h"

namespace Fuego::Renderer
{
class Surface;

class CommandBufferOpenGL final : public CommandBuffer
{
public:
    virtual ~CommandBufferOpenGL() override;
    virtual void BeginRecording() override;
    virtual void EndRecording() override;
    virtual void Submit() override;
    virtual void BindRenderTarget(const Surface& texture) override;
    virtual void BindVertexShader(const Shader& vertexShader) override;
    virtual void BindPixelShader(const Shader& pixelShader) override;
    virtual void BindDescriptorSet(const DescriptorBuffer& descriptorSet, int setIndex) override;
    virtual void BindVertexBuffer(const Buffer& vertexBuffer) override;
    virtual void BindIndexBuffer(uint32_t indices[], uint32_t size) override;
    virtual void BindTexture(unsigned char* data, int w, int h) override;
    virtual void Draw(uint32_t vertexCount) override;
    virtual void IndexedDraw(uint32_t vertexCount) override;
    virtual void Clear() override;

private:
    uint32_t _vao;
    uint32_t _ebo;
    uint32_t _programID;
    uint32_t _vertexShader;
    uint32_t _pixelShader;
    bool _isLinked;
    bool _isDataAllocated;

    uint32_t _texture;

    friend class CommandPoolOpenGL;
    bool _isFree;
    CommandBufferOpenGL();
};
}  // namespace Fuego::Renderer
