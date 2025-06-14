#include "BufferOpenGL.h"

#include "../../Renderer/Graphics.hpp"
#include "glad/gl.h"

namespace Fuego::Graphics
{

BufferOpenGL::BufferOpenGL(RenderStage stage, size_t size_bytes, size_t offset) : Buffer(size_bytes), _vbo(UINT32_MAX)
{
    FU_CORE_ASSERT(size_bytes > 0, "Buffer can't be 0 sized");

    glGenBuffers(1, &_vbo);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, size_bytes, nullptr, ConvertUsage(stage));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

BufferOpenGL::~BufferOpenGL()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &_vbo);
}

void BufferOpenGL::UpdateSubDataImpl(const void* data, size_t size_bytes, size_t offset)
{
    FU_CORE_ASSERT(last_buffered_idx + size_bytes < end_idx, "Buffer overflow");

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferSubData(GL_ARRAY_BUFFER, last_buffered_idx + offset, size_bytes, data);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    last_buffered_idx += size_bytes;
    UNUSED(offset);
}

uint32_t BufferOpenGL::GetBufferID() const
{
    return _vbo;
}

int BufferOpenGL::ConvertUsage(RenderStage& stage) const
{
    switch (stage)
    {
        case STATIC_GEOMETRY:
            return GL_STATIC_DRAW;
        case DYNAMIC_DRAW:
            return GL_DYNAMIC_DRAW;
    }
}
}  // namespace Fuego::Graphics
