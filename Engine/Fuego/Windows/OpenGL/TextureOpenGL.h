#pragma once

#include "Renderer/Texture.h"
#include "glad/gl.h"

namespace Fuego::Renderer
{
class TextureOpenGL : public Texture
{
public:
    static Texture* CreateTexture(unsigned char* data, int width, int heigth);
    ~TextureOpenGL() = default;

    virtual void Bind() override;
    virtual TextureFormat GetTextureFormat() const override;

private:
    friend class Texture;
    TextureOpenGL(unsigned char* data, int width, int heigth);
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
