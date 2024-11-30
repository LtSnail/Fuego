#pragma once
#include <string>

namespace Fuego
{
class Shader
{
public:
    Shader() = default;
    virtual ~Shader();

    virtual void Bind() const;

private:
    uint32_t _shaderID;
};
}  // namespace Fuego
