#pragma once
#include <string>

namespace Fuego
{
class Shader
{
public:
    virtual ~Shader() = default;

    virtual void Bind() const = 0;
};
}  // namespace Fuego
