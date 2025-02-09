#pragma once

namespace Fuego::Renderer
{
class Surface
{
public:
    struct Rect
    {
        int64_t x, y, width, height;
    };

    virtual ~Surface() = default;
    virtual Rect GetRect() const = 0;

    virtual const void* GetNativeHandle() const = 0;
};
}  // namespace Fuego::Renderer
