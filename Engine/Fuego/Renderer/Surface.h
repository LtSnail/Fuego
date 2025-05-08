#pragma once

namespace Fuego::Graphics
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

    virtual void Clear() const = 0;
};
}  // namespace Fuego::Graphics
