#pragma once

#include <span>

namespace Fuego::Graphics
{
class Buffer
{
   public:
    virtual ~Buffer() = default;

    void UpdateSubData(const void* data, size_t size_bytes, size_t offset = 0)
    {
        UpdateSubDataImpl(data, size_bytes, offset);
    }

   protected:
    virtual void UpdateSubDataImpl(const void* data, size_t size_bytes, size_t offset) = 0;

    Buffer(size_t size_bytes) : end_idx(size_bytes), last_buffered_idx(0) {};
    uint32_t end_idx;
    uint32_t last_buffered_idx;
};
}  // namespace Fuego::Graphics
