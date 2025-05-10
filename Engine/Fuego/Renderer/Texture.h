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
        delete data;
        data = nullptr;
    }

    virtual TextureFormat GetTextureFormat() const = 0;
    virtual inline const unsigned char* Data() const
    {
        return data;
    }
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

private:
    unsigned char* data;
    std::string name;
    int width;
    int height;
};  // namespace Fuego::Graphics

class TextureView
{
public:
    virtual ~TextureView() = default;
};
}  // namespace Fuego::Graphics
