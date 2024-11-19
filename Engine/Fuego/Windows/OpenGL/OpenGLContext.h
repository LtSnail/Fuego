#pragma once
// clang-format off
#include "Renderer/GraphicsContext.h"
#include "fupch.h"
#include "glad/glad.h"
// clang-format on
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