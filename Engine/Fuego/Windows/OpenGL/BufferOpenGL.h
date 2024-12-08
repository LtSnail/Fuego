#pragma once

#include "Renderer/Buffer.h"

namespace Fuego::Renderer
{
class BufferOpenGL : public Buffer
{
public:
    BufferOpenGL(size_t size, uint32_t flags);
    virtual ~BufferOpenGL();
    uint32_t GetBufferID() const;

protected:

private:
    uint32_t _vbo;
};
}  // namespace Fuego::Renderer
