#pragma once

#include "glm/glm.hpp"

namespace Fuego::Graphics
{
class ShaderObject;
class Texture;

class Shader
{
public:
    enum ShaderType
    {
        Vertex = 1,
        Pixel = 2
    };


    friend class ShaderObject;
    virtual void BindToShaderObject(ShaderObject& obj) = 0;
    virtual bool AddVar(const std::string& name) = 0;
    virtual bool SetVec3f(const std::string& var, glm::vec3 vector) const = 0;
    virtual bool SetMat4f(const std::string& var, glm::mat4 matrix) const = 0;
    virtual bool SetText2D(const std::string& var, const Texture& texture) const = 0;

    virtual ~Shader() = default;
};
}  // namespace Fuego::Graphics
