#pragma once

#include "Renderer/Buffer.h"

namespace Fuego::Renderer
{
class BufferOpenGL : public Buffer
{
public:
    BufferOpenGL(size_t size, uint32_t flags);
    ~BufferOpenGL();

    uint32_t GetBufferID() const;
    virtual void BindDataImpl(const void* data, size_t size, size_t offset) override;

private:
    uint32_t _vbo;
};
}  // namespace Fuego::Renderer
