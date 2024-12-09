#pragma once

namespace Fuego::Renderer
{
class Surface
{
public:
    virtual ~Surface() = default;

    virtual const void* GetNativeHandle() const = 0;
};
}  // namespace Fuego::Renderer
