#pragma once

#include "glm/common.hpp"

namespace Fuego::Graphics
{
class ShaderObject;
class Texture;

class FUEGO_API Material
{
public:
    static Material* CreateMaterial(const Texture* albedo);

    virtual ~Material() = default;
    
    virtual void Use() const = 0;
    virtual inline std::string_view Name() const
    {
        return name;
    }

private:
    std::string name;
};
}  // namespace Fuego::Graphics
