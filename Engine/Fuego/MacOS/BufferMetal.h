#pragma once

#include "../Renderer/Buffer.h"

namespace Fuego
{
class VertexBufferMetal : VertexBuffer
{
public:
    VertexBufferMetal();
    virtual ~VertexBufferMetal() override;

    virtual void Bind() const override;
    virtual void Unbind() const override;

private:
};

class IndexBufferMetal : IndexBuffer
{
public:
    IndexBufferMetal() = default;
    virtual ~IndexBufferMetal();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual uint32_t GetCount() const override;
};
}  // namespace Fuego
