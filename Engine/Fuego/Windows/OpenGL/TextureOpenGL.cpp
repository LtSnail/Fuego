#include "TextureOpenGL.h"

namespace Fuego::Renderer
{
TextureOpenGL::TextureOpenGL(unsigned char* data, int width, int heigth)
    : _textureTarget(GL_TEXTURE_2D)
    , _texture(0)
{
    glGenTextures(1, &_texture);
    glBindTexture(_textureTarget, _texture);
    glTexImage2D(_textureTarget, 0, GL_RGB, width, heigth, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    // Configuration of minification/Magnification
    glTexParameterf(_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(_textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(_textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(_texture, 0);
}

Texture* Texture::CreateTexture(unsigned char* data, int width, int heigth)
{
    return new TextureOpenGL(data, width, heigth);
}

void TextureOpenGL::Bind()
{
    glActiveTexture(_texture);
    glBindTexture(_textureTarget, _texture);
}

TextureFormat TextureOpenGL::GetTextureFormat() const
{
    return TextureFormat::R16F;
}
}  // namespace Fuego::Renderer
