#pragma once

namespace Fuego::Renderer
{
class Surface
{
public:
    virtual ~Surface() = default;

    virtual void* GetNativeHandle() const = 0;
};
}  // namespace Fuego::Renderer
