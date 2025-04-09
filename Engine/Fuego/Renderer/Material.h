#pragma once

#include "glm/common.hpp"

namespace Fuego::Renderer
{
class ShaderObject;
class Texture;

class FUEGO_API Material
{
public:
    static Material* CreateMaterial(Texture* albedo);

    virtual ~Material() = default;

    virtual void Upload(ShaderObject& shader) = 0;

private:
};
}  // namespace Fuego::Renderer
