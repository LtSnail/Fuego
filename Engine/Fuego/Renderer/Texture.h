#pragma once

namespace Fuego::Graphics
{
enum class TextureFormat
{
    R8,       // Single-channel 8-bit
    RG8,      // Two-channel 8-bit
    RGBA8,    // Four-channel 8-bit
    RGB8,     // Three-channel 8-bit
    R16F,     // Single-channel 16-bit floating point
    RG16F,    // Two-channel 16-bit floating point
    RGBA16F,  // Four-channel 16-bit floating point
    R32F,     // Single-channel 32-bit floating point
    RGBA32F   // Four-channel 32-bit floating point
};

class Texture
{
public:
    virtual ~Texture()
    {
    }

    virtual TextureFormat GetTextureFormat() const = 0;
    virtual inline std::string_view Name() const
    {
        return name;
    }
    virtual inline int Width() const
    {
        return width;
    }
    virtual inline int Height() const
    {
        return height;
    }

    virtual void Bind() const = 0;
    virtual void UnBind() const = 0;

    static TextureFormat GetTextureFormat(uint16_t channels, uint16_t bpp)
    {
        if (channels == 3)
        {
            if (bpp <= 8)
                return TextureFormat::RGB8;
            else if (bpp == 16)
                return TextureFormat::RGBA16F;
        }
        else if (channels == 4)
        {
            if (bpp <= 8)
                return TextureFormat::RGBA8;
            else if (bpp == 16)
                return TextureFormat::RGBA16F;
        }
    }

protected:
    Texture(std::string_view name, TextureFormat format, int width, int height)
        : name(name)
        , format(format)
        , width(width)
        , height(height)
    {
    }

private:
    std::string name;
    int width;
    int height;
    TextureFormat format;
};  // namespace Fuego::Graphics

class TextureView
{
public:
    virtual ~TextureView() = default;
};
}  // namespace Fuego::Graphics
