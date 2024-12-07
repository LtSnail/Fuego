#pragma once

namespace Fuego::Renderer
{
class Buffer
{
public:
    virtual ~Buffer() = default;
    static std::unique_ptr<Buffer> Create(size_t size, uint32_t flags);
};
}  // namespace Fuego::Renderer
