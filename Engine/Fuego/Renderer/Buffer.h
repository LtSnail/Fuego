#pragma once

namespace Fuego::Renderer
{
class Buffer
{
public:
    static std::unique_ptr<Buffer> Create(size_t size, uint32_t flags);
    virtual ~Buffer() = default;
};
}  // namespace Fuego::Renderer
