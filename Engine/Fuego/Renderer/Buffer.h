#pragma once

#include "fupch.h"

namespace Fuego
{
class VertexBuffer
{
public:
    VertexBuffer() = default;
    virtual ~VertexBuffer() {};

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    static VertexBuffer* Create(float* vertices, uint32_t size);
};

class IndexBuffer
{
public:
    IndexBuffer() = default;
    virtual ~IndexBuffer() {};

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual uint32_t GetCount() const = 0;

    static IndexBuffer* Create(uint32_t* indices, uint32_t count);
};
}  // namespace Fuego
