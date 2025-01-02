#pragma once

#include "Renderer/Swapchain.h"
#include "SurfaceOpenGL.h"
#include "glad/gl.h"

namespace Fuego::Renderer
{
class Surface;

class SwapchainOpenGL : public Swapchain
{
public:
    virtual ~SwapchainOpenGL() override;

    virtual void Present() override;

    virtual Surface& GetScreenTexture() override;

    virtual void ShowWireFrame(bool show) override;

private:
    static void APIENTRY OpenGLDebugCallbackFunc(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message,
                                                 const void* userParam);
    SurfaceOpenGL _surface;
    HGLRC _ctx;

protected:
    friend class DeviceOpenGL;
    SwapchainOpenGL(const Surface& surface);
};
}  // namespace Fuego::Renderer
