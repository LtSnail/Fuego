#include "TextureOpenGL.h"

#include "glad/gl.h"

namespace Fuego::Graphics
{
TextureOpenGL::TextureOpenGL(unsigned char* buffer, int width, int heigth)
    : texture_unit(0)
    , texture_id(0)
{
    FU_CORE_ASSERT(buffer, "Texture buffer is empty");

    glBindTexture(GL_TEXTURE_2D, 0);
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, heigth, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);

    glGenerateMipmap(GL_TEXTURE_2D);

    // Configuration of minification/Magnification
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);
}

TextureOpenGL::~TextureOpenGL()
{
    if (texture_id != 0)
        glDeleteTextures(1, &texture_id);
}

void TextureOpenGL::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, texture_id);
}

void TextureOpenGL::UnBind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

TextureFormat TextureOpenGL::GetTextureFormat() const
{
    return TextureFormat::R16F;
}
}  // namespace Fuego::Graphics
