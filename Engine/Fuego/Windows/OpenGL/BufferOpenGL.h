#pragma once

#include "Renderer/Buffer.h"

namespace Fuego::Renderer
{
class BufferOpenGL : public Buffer
{
public:
    virtual ~BufferOpenGL();
    uint32_t GetBufferID() const;
    static std::unique_ptr<Buffer> Create(size_t size, uint32_t flags);

protected:
    BufferOpenGL(size_t size, uint32_t flags);

private:
    uint32_t _vbo;
};
}  // namespace Fuego::Renderer
