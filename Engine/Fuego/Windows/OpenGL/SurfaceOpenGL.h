#pragma once

#include "Renderer/Surface.h"

namespace Fuego::Graphics
{
class SurfaceOpenGL final : public Surface
{
public:
    SurfaceOpenGL(const void* window);
    virtual ~SurfaceOpenGL() override;
    virtual Rect GetRect() const override;

    virtual const void* GetNativeHandle() const override;
    HDC GetHdc() const;

    virtual void Clear() const override;

    virtual void Release() override;

private:
    HWND _window;  // shouldn't be here
    HDC _hdc;      // WIN32: Handle to Device Context
};
}  // namespace Fuego::Graphics
