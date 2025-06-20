#pragma once

#include "Color.h"
#include "Image2D.h"

namespace Fuego::Graphics
{

enum class TextureFormat
{
    R8,       // Single-channel 8-bit
    RG8,      // Two-channel 8-bit
    RGB8,     // Three-channel 8-bit
    RGBA8,    // Four-channel 8-bit
    R16F,     // Single-channel 16-bit floating point
    RG16F,    // Two-channel 16-bit floating point
    RGB16F,   // Three-channel 16-bit floating point
    RGBA16F,  // Four-channel 16-bit floating point
    R32F,     // Single-channel 32-bit floating point
    RGBA32F,  // Four-channel 32-bit floating point
    NONE
};

class Texture
{
public:
    virtual ~Texture() = default;

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
        switch (channels)
        {
        case 1:
            if (bpp <= 8)
                return TextureFormat::R8;
            break;

        case 2:
            if (bpp <= 8)
                return TextureFormat::RG8;
            break;

        case 3:
            if (bpp <= 8)
                return TextureFormat::RGB8;
            break;

        case 4:
            if (bpp <= 8)
                return TextureFormat::RGBA8;
            break;
        }
    }

    inline bool IsValid() const
    {
        return is_created;
    }

    virtual void PostCreate(std::shared_ptr<Fuego::Graphics::Image2D> img)
    {
        const auto& image = *img.get();
        width = image.Width();
        height = image.Height();
        format = Fuego::Graphics::Texture::GetTextureFormat(image.Channels(), image.BBP());
        is_created = true;
    }

protected:
    Texture(std::string_view name, TextureFormat format, int width, int height)
        : name(name)
        , format(format)
        , width(width)
        , height(height)
        , is_created(false) {};

    Texture(std::string_view name)
        : name(name)
        , format(TextureFormat::NONE)
        , width(0)
        , height(0)
        , is_created(false)
    {
    }

    bool is_created;
    std::string name;
    int width;
    int height;
    TextureFormat format;

    uint32_t calculate_mipmap_level(uint32_t width, uint32_t height) const
    {
        return 1 + static_cast<uint32_t>(std::floor(std::log2(std::max(width, height))));
    }
    uint32_t get_channels(TextureFormat format) const
    {
        switch (format)
        {
        case Fuego::Graphics::TextureFormat::R8:
        case Fuego::Graphics::TextureFormat::R16F:
        case Fuego::Graphics::TextureFormat::R32F:
            return 1;

        case Fuego::Graphics::TextureFormat::RG8:
        case Fuego::Graphics::TextureFormat::RG16F:
            return 2;

        case Fuego::Graphics::TextureFormat::RGB8:
        case Fuego::Graphics::TextureFormat::RGB16F:
            return 3;

        case Fuego::Graphics::TextureFormat::RGBA8:
        case Fuego::Graphics::TextureFormat::RGBA16F:
        case Fuego::Graphics::TextureFormat::RGBA32F:
            return 4;
        }
    }
};  // namespace Fuego::Graphics

class TextureView
{
public:
    virtual ~TextureView() = default;
};
}  // namespace Fuego::Graphics
