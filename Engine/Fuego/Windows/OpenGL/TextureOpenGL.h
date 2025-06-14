#pragma once

#include "Renderer/Texture.h"

namespace Fuego::Graphics
{
class TextureOpenGL final : public Texture
{
   public:
    // TODO move ctors from public to private?
    friend class DeviceOpenGL;
    TextureOpenGL(std::string_view name);
    TextureOpenGL(std::string_view name, TextureFormat format, unsigned char* buffer, int width, int height);
    TextureOpenGL(std::string_view name, TextureFormat format, Color color, int width, int height);

    virtual ~TextureOpenGL() override;

    virtual void Bind() const override;
    virtual void UnBind() const override;

    virtual TextureFormat GetTextureFormat() const override;
    inline uint16_t GetTextureUnit() const { return texture_unit; }
    inline uint32_t GetTextureID() const { return texture_id; }

   private:
    uint32_t ColorFormat(TextureFormat format);
    uint32_t PixelFormat(uint16_t channels, bool inverted = false);
    uint32_t PixelFormat(TextureFormat format, bool inverted = false);

    uint16_t texture_unit;
    uint32_t texture_id;

    virtual void PostCreate(std::shared_ptr<Fuego::Graphics::Image2D> img) override;
};

class TextureViewOpenGL
{
   public:
    TextureViewOpenGL() = default;
    ~TextureViewOpenGL() = default;
};
}  // namespace Fuego::Graphics
