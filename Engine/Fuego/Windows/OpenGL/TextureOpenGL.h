#pragma once

#include "Renderer/Texture.h"
#include "glad/gl.h"

namespace Fuego::Renderer
{
class TextureOpenGL : public Texture
{
public:
    TextureOpenGL(unsigned int* data, int width, int heigth);
    ~TextureOpenGL() = default;

    virtual void Bind() override;
    virtual TextureFormat GetTextureFormat() const override;

private:
    //std::string _fileName;
    GLuint _texture;
    GLenum _textureTarget;
};

class TextureViewOpenGL
{
public:
    TextureViewOpenGL() = default;
    ~TextureViewOpenGL() = default;
};
}  // namespace Fuego::Renderer
