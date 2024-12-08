#pragma once

namespace Fuego::Renderer
{
class Surface
{
public:
    static std::unique_ptr<Surface> CreateSurface(void* handle);
    virtual ~Surface() = default;

    virtual const void* GetNativeHandle() const = 0;
};
}  // namespace Fuego::Renderer
