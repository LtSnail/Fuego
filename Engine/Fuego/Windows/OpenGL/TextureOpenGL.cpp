#include "TextureOpenGL.h"

#include "glad/wgl.h"

namespace Fuego::Graphics
{

TextureOpenGL::TextureOpenGL(std::string_view name) : Texture(name), texture_unit(0), texture_id(0)
{
    is_created = false;
}

TextureOpenGL::TextureOpenGL(std::string_view name, TextureFormat format, unsigned char* buffer, int width, int height)
    : Texture(name, format, width, height), texture_unit(0), texture_id(0)
{
    FU_CORE_ASSERT(buffer, "Texture buffer is empty");

    glCreateTextures(GL_TEXTURE_2D, 1, &texture_id);
    FU_CORE_ASSERT(!texture_id == 0, "Texture didn't create");

    uint32_t mipmap_levels = calculate_mipmap_level(width, height);
    glTextureStorage2D(texture_id, mipmap_levels, ColorFormat(format), width, height);
    glTextureSubImage2D(texture_id, 0, 0, 0, width, height, PixelFormat(format), GL_UNSIGNED_BYTE, buffer);

    glGenerateTextureMipmap(texture_id);

    glTextureParameteri(texture_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(texture_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(texture_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(texture_id, GL_TEXTURE_WRAP_T, GL_REPEAT);

    is_created = true;
}

TextureOpenGL::TextureOpenGL(std::string_view name, TextureFormat format, Color color, int width, int height)
    : Texture(name, format, width, height)
{
    glCreateTextures(GL_TEXTURE_2D, 1, &texture_id);
    FU_CORE_ASSERT(!texture_id == 0, "Texture didn't create");

    unsigned char* data = new unsigned char[width * height * 4];
    std::fill(data, data + width * height * 4 - 1, color.Data());

    uint32_t mipmap_levels = calculate_mipmap_level(width, height);
    glTextureStorage2D(texture_id, mipmap_levels, ColorFormat(format), width, height);
    glTextureSubImage2D(texture_id, 0, 0, 0, width, height, PixelFormat(format), GL_UNSIGNED_BYTE, data);

    glGenerateTextureMipmap(texture_id);

    // Configuration of minification/Magnification
    glTextureParameteri(texture_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(texture_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(texture_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(texture_id, GL_TEXTURE_WRAP_T, GL_REPEAT);

    delete data;
    is_created = true;
}

void TextureOpenGL::PostCreate(std::shared_ptr<Fuego::Graphics::Image2D> img)
{
    const auto& image = *img.get();
    FU_CORE_ASSERT(image.Data()[0] != '\n' || image.Data()[0] != ' ' || image.IsValid(),
                   "[TextureOpenGL->PostCreate] broken image2d data");

    Texture::PostCreate(img);

    glCreateTextures(GL_TEXTURE_2D, 1, &texture_id);
    FU_CORE_ASSERT(!texture_id == 0, "Texture didn't create");

    uint32_t mipmap_levels = calculate_mipmap_level(width, height);
    glTextureStorage2D(texture_id, mipmap_levels, ColorFormat(format), width, height);
    glTextureSubImage2D(texture_id, 0, 0, 0, width, height, PixelFormat(format), GL_UNSIGNED_BYTE, image.Data());

    glGenerateTextureMipmap(texture_id);

    // Configuration of minification/Magnification
    glTextureParameteri(texture_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(texture_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(texture_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(texture_id, GL_TEXTURE_WRAP_T, GL_REPEAT);

    is_created = true;
}

uint32_t TextureOpenGL::ColorFormat(TextureFormat format)
{
    switch (format)
    {
        case Fuego::Graphics::TextureFormat::R8:
            return GL_R8;
        case Fuego::Graphics::TextureFormat::RG8:
            return GL_RG8;
        case Fuego::Graphics::TextureFormat::RGB8:
            return GL_RGB8;
        case Fuego::Graphics::TextureFormat::RGBA8:
            return GL_RGBA8;

        case Fuego::Graphics::TextureFormat::R16F:
            return GL_R16F;
        case Fuego::Graphics::TextureFormat::RG16F:
            return GL_RG16F;
        case Fuego::Graphics::TextureFormat::RGB16F:
            return GL_RGB16F;
        case Fuego::Graphics::TextureFormat::RGBA16F:
            return GL_RGBA16F;

        case Fuego::Graphics::TextureFormat::R32F:
            return GL_R32F;
        case Fuego::Graphics::TextureFormat::RGBA32F:
            return GL_RGBA32F;
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

void TextureOpenGL::Bind() const {}

void TextureOpenGL::UnBind() const {}

TextureFormat TextureOpenGL::GetTextureFormat() const
{
    return TextureFormat::R16F;
}
}  // namespace Fuego::Graphics
