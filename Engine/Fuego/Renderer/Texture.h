#pragma once

namespace Fuego::Renderer
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
    virtual ~Texture() = default;

    virtual TextureFormat GetTextureFormat() const = 0;
};

class TextureView
{
public:
    virtual ~TextureView() = default;
};
}  // namespace Fuego::Renderer
