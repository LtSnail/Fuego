#include "TextureStab.h"

namespace Fuego::Renderer
{
TextureStab::~TextureStab()
{
}

TextureFormat TextureStab::GetTextureFormat() const
{
    return TextureFormat::R16F;
}

}  // namespace Fuego::Renderer