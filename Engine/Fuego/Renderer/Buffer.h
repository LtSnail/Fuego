#pragma once

#include <span>

namespace Fuego::Renderer
{
class Buffer
{
public:
    static std::unique_ptr<Buffer> Create(size_t size, uint32_t flags);
    virtual ~Buffer() = default;

    template <typename T>
    void BindData(std::span<const T> data, size_t offset = 0)
    {
        BindDataImpl(data.data(), data.size_bytes(), offset);
    }

protected:
    virtual void BindDataImpl(const void* data, size_t size, size_t offset) = 0;
};
}  // namespace Fuego::Renderer
