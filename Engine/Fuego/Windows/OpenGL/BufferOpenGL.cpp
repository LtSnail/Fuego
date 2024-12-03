#include "BufferOpenGL.h"

#include "glad/glad.h"

namespace Fuego::Renderer
{

BufferOpenGL::BufferOpenGL(size_t size, uint32_t flags)
    : _vbo(0)
{
    UNUSED(flags);

    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
}

BufferOpenGL::~BufferOpenGL()
{
    glDeleteBuffers(1, &_vbo);
}

uint32_t BufferOpenGL::GetBufferID() const
{
    _vbo;
}
}  // namespace Fuego::Renderer
