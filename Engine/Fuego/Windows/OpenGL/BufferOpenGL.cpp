#include "BufferOpenGL.h"

namespace Fuego
{
bool VertexBufferOpenGL::_isVAO = false;

VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
{
    return new VertexBufferOpenGL(vertices, size);
}

VertexBufferOpenGL::VertexBufferOpenGL(float* vertices, uint32_t size)
    : _VBO(0)
    , _VAO(0)
{
    if (!_isVAO)
    {
        glGenVertexArrays(1, &_VAO);
        glBindVertexArray(_VAO);
        _isVAO = true;
    }
    glGenBuffers(1, &_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
}

VertexBufferOpenGL::~VertexBufferOpenGL()
{
    glDeleteBuffers(1, &_VBO);
}

void VertexBufferOpenGL::Bind() const
{
}

void VertexBufferOpenGL::Unbind() const
{
}

//////////////////////////////////////////////////////////////
/// IndexBuffer /////////////////////////////////////////////
////////////////////////////////////////////////////////////

IndexBufferOpenGL::IndexBufferOpenGL(uint32_t* indices, uint32_t count)
    : _EBO(0)
    , _count(count)
{
    glGenBuffers(1, &_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}

IndexBufferOpenGL::~IndexBufferOpenGL()
{
}

void IndexBufferOpenGL::Bind() const
{
}

void IndexBufferOpenGL::Unbind() const
{
}

IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
{
    return new IndexBufferOpenGL(indices, count);
}

}  // namespace Fuego
