#include "BufferOpenGL.h"

namespace Fuego
{
VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
{
    UNUSED(vertices);
    UNUSED(size);
    return nullptr;
}

VertexBufferOpenGL::VertexBufferOpenGL() {}

VertexBufferOpenGL::~VertexBufferOpenGL() {}
void VertexBufferOpenGL::Bind() const {}
void VertexBufferOpenGL::Unbind() const {}

IndexBufferOpenGL::~IndexBufferOpenGL() {}

void IndexBufferOpenGL::Bind() const {}

void IndexBufferOpenGL::Unbind() const {}

uint32_t IndexBufferOpenGL::GetCount() const { return 0; }

IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size)
{
    UNUSED(indices);
    UNUSED(size);
    return nullptr;
}

}  // namespace Fuego