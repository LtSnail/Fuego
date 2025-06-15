#pragma once

#include <span>

namespace Fuego::Graphics
{
class Buffer
{
   public:
    virtual ~Buffer() = default;

    enum BufferType
    {
        Vertex,
        Index
    };

    uint32_t UpdateSubData(const void* data, size_t size_bytes) { return UpdateSubDataImpl(data, size_bytes); }
    virtual uint32_t NativeType() const = 0;

   protected:
    virtual uint32_t UpdateSubDataImpl(const void* data, size_t size_bytes) = 0;

    Buffer(BufferType type, size_t size_bytes) : type(type), end_idx(size_bytes), last_buffered_idx_to_byte(0) {};
    uint32_t end_idx;
    uint32_t last_buffered_idx_to_byte;
    BufferType type;
};
}  // namespace Fuego::Graphics
