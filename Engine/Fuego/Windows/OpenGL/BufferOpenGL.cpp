#include "BufferOpenGL.h"

#include "glad/gl.h"

namespace Fuego::Graphics
{

BufferOpenGL::BufferOpenGL()
    : _vbo(UINT32_MAX)
{
    glGenBuffers(1, &_vbo);
}

BufferOpenGL::~BufferOpenGL()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &_vbo);
}

uint32_t BufferOpenGL::GetBufferID() const
{
    return _vbo;
}

void BufferOpenGL::BindDataImpl(const void* data, size_t size_bytes, size_t offset)
{
    UNUSED(offset);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, size_bytes, data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
}  // namespace Fuego::Graphics
