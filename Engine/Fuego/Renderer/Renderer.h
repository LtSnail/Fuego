#pragma once

namespace Fuego
{
// clang-format off
enum RendererAPI
{    
    None        = 0,
    OpenGL      = 1,
    Metal       = 2,
    DerectX12   = 3
};
// clang-format on

class Renderer
{
   public:
    static inline RendererAPI GetAPI() { return _rendererAPI; }

   private:
    static RendererAPI _rendererAPI;
};
}  // namespace Fuego
