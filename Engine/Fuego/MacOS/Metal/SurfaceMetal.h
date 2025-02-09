#pragma once

#include <Metal/Metal.hpp>

#include "Renderer/Surface.h"


namespace MTL
{
class Device;
}

namespace Fuego::Renderer
{
class SurfaceMetal : public Surface
{
public:
    SurfaceMetal(const void* window, MTL::Device* device);
    ~SurfaceMetal();

    virtual Rect GetRect() const override
    {
        return {0, 0, 0, 0};
    }

    virtual const void* GetNativeHandle() const override;
    MTL::PixelFormat GetPixelFormat() const;

private:
    void* _metalLayer;
};
}  // namespace Fuego::Renderer
