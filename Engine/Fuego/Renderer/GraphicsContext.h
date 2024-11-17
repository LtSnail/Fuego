#pragma once

namespace Fuego
{
class GraphicsContext
{
   public:
    virtual bool Init() = 0;
    virtual void SwapBuffers() = 0;
    virtual ~GraphicsContext() = default;
};
}  // namespace Fuego