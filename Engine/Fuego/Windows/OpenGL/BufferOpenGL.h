#pragma once

#include "Renderer/Buffer.h"
#include "glad/glad.h"

namespace Fuego
{
class VertexBufferOpenGL : VertexBuffer
{
   public:
    VertexBufferOpenGL();
    virtual ~VertexBufferOpenGL() override;

    virtual void Bind() const override;
    virtual void Unbind() const override;

   private:
};

class IndexBufferOpenGL : IndexBuffer
{
   public:
    IndexBufferOpenGL() = default;
    virtual ~IndexBufferOpenGL();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual uint32_t GetCount() const override;
};
}  // namespace Fuego