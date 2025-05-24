#include "TextureOpenGL.h"

#include "glad/wgl.h"

namespace Fuego::Graphics
{
TextureOpenGL::TextureOpenGL(std::string_view name, TextureFormat format, unsigned char* buffer, int width, int height)
    : Texture(name, format, width, height)
    , texture_unit(0)
    , texture_id(0)
{
    FU_CORE_ASSERT(buffer, "Texture buffer is empty");

    glBindTexture(GL_TEXTURE_2D, 0);
    glGenTextures(1, &texture_id);
    FU_CORE_ASSERT(!texture_id == 0, "Texture didn't create");

    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, ColorFormat(format), width, height, 0, PixelFormat(format), GL_UNSIGNED_BYTE, buffer);

    glGenerateMipmap(GL_TEXTURE_2D);

    // Configuration of minification/Magnification
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, 0);
}

uint32_t TextureOpenGL::ColorFormat(TextureFormat format)
{
    switch (format)
    {
    case Fuego::Graphics::TextureFormat::R8:
        return GL_R8;
        break;
    case Fuego::Graphics::TextureFormat::RG8:
        return GL_RG8;
        break;
    case Fuego::Graphics::TextureFormat::RGBA8:
        return GL_RGBA8;
        break;
    case Fuego::Graphics::TextureFormat::RGB8:
        return GL_RGB8;
        break;
    case Fuego::Graphics::TextureFormat::R16F:
        return GL_R16F;
        break;
    case Fuego::Graphics::TextureFormat::RG16F:
        return GL_RG16F;
        break;
    case Fuego::Graphics::TextureFormat::RGBA16F:
        return GL_RGBA16F;
        break;
    case Fuego::Graphics::TextureFormat::R32F:
        return GL_R32F;
        break;
    case Fuego::Graphics::TextureFormat::RGBA32F:
        return GL_RGBA32F;
        break;
    }
}

uint32_t TextureOpenGL::PixelFormat(uint16_t channels, bool inverted)
{
    switch (channels)
    {
    case 1:
        return GL_RED;
    case 2:
        return GL_RG;
    case 3:
        return inverted ? GL_BGR : GL_RGB;
    case 4:
        return inverted ? GL_BGRA : GL_RGBA;
    default:
        FU_CORE_ASSERT(false, "Unsupported channel count");
        return GL_RGBA;
    }
}

uint32_t TextureOpenGL::PixelFormat(TextureFormat format, bool inverted)
{
    switch (format)
    {
    case TextureFormat::R8:
    case TextureFormat::R16F:
    case TextureFormat::R32F:
        return GL_RED;

    case TextureFormat::RG8:
    case TextureFormat::RG16F:
        return GL_RG;

    case TextureFormat::RGB8:
        return inverted ? GL_BGR : GL_RGB;

    case TextureFormat::RGBA8:
    case TextureFormat::RGBA16F:
    case TextureFormat::RGBA32F:
        return inverted ? GL_BGRA : GL_RGBA;

    default:
        FU_CORE_ASSERT(false, "Unsupported TextureFormat in PixelFormat");
        return GL_RGBA;
    }
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
