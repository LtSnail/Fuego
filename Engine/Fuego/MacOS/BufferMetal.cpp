#include "BufferMetal.h"

namespace Fuego
{
VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
{
    UNUSED(vertices);
    UNUSED(size);
    return nullptr;
}

VertexBufferMetal::VertexBufferMetal() {}

VertexBufferMetal::~VertexBufferMetal() {}
void VertexBufferMetal::Bind() const {}
void VertexBufferMetal::Unbind() const {}

IndexBufferMetal::~IndexBufferMetal() {}

void IndexBufferMetal::Bind() const {}

void IndexBufferMetal::Unbind() const {}

uint32_t IndexBufferMetal::GetCount() const { return 0; }

IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size)
{
    UNUSED(indices);
    UNUSED(size);
    return nullptr;
}

}  // namespace Fuego