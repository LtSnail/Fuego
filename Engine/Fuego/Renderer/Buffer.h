#pragma once

#include <span>

namespace Fuego::Graphics
{
class Buffer
{
public:
    virtual ~Buffer() = default;

    template <typename T>
    void BindData(std::span<const T> data, size_t offset = 0)
    {
        BindDataImpl(data.data(), data.size_bytes(), offset);
    }

protected:
    virtual void BindDataImpl(const void* data, size_t size_bytes, size_t offset) = 0;
};
}  // namespace Fuego::Graphics
