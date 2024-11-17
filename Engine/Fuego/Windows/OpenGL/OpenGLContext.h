#pragma once

#include "Renderer/GraphicsContext.h"

namespace Fuego
{
class OpenGLContext : public GraphicsContext
{
   public:
    OpenGLContext(HWND* windowHandle);
    virtual ~OpenGLContext() override;

    virtual bool Init() override;
    virtual void SwapBuffers() override;

   private:
    HWND* _windowHandle;
    HGLRC _openGLContext;
};
}  // namespace Fuego