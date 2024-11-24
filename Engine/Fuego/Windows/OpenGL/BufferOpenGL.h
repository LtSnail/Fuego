#pragma once

#include "Renderer/Buffer.h"
#include "glad/glad.h"

namespace Fuego
{
class VertexBufferOpenGL : public VertexBuffer
{
public:
    VertexBufferOpenGL(float* vertices, uint32_t size);
    virtual ~VertexBufferOpenGL() override;

    virtual void Bind() const override;
    virtual void Unbind() const override;

private:
    static bool _isVAO;
    uint32_t _VBO, _VAO;
};

class IndexBufferOpenGL : public IndexBuffer
{
public:
    IndexBufferOpenGL(uint32_t* indices, uint32_t count);
    virtual ~IndexBufferOpenGL();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual uint32_t GetCount() const override
    {
        return _count;
    };

private:
    uint32_t _EBO;
    uint32_t _count;
};
}  // namespace Fuego
