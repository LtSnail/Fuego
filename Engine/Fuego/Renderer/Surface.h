#pragma once

namespace Fuego::Renderer
{
class Window;

class Surface
{
public:
    virtual ~Surface() = default;

    virtual Window& GetBindedWindow() const = 0;
};
}  // namespace Fuego::Renderer
