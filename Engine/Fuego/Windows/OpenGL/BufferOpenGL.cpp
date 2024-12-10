#include "BufferOpenGL.h"

#include "glad/glad.h"

namespace Fuego::Renderer
{

BufferOpenGL::BufferOpenGL(size_t size, uint32_t flags)
    : _vbo(UINT32_MAX)
{
    UNUSED(flags);

    glGenBuffers(1, &_vbo);
}

BufferOpenGL::~BufferOpenGL()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

uint32_t BufferOpenGL::GetBufferID() const
{
    return _vbo;
}

void BufferOpenGL::BindDataImpl(const void* data, size_t size, size_t offset)
{
    UNUSED(offset);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}
}  // namespace Fuego::Renderer
