#pragma once

#include "Renderer/Buffer.h"

namespace Fuego::Graphics
{
enum RenderStage;

class BufferOpenGL final : public Buffer
{
   public:
    virtual ~BufferOpenGL() override;

    uint32_t GetBufferID() const;
    virtual uint32_t UpdateSubDataImpl(const void* data, size_t size) override;
    virtual uint32_t NativeType() const override;

   private:
    uint32_t buffer_object_id;
    uint32_t buffer_native_type;

   protected:
    friend class DeviceOpenGL;
    int native_usage(RenderStage& stage) const;
    int native_buffer_type(const BufferType& type) const;
    BufferOpenGL(BufferType type, RenderStage stage, size_t size_bytes);
};
}  // namespace Fuego::Graphics
