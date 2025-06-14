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
    virtual void UpdateSubDataImpl(const void* data, size_t size, size_t offset) override;

   private:
    uint32_t _vbo;

   protected:
    friend class DeviceOpenGL;
    int ConvertUsage(RenderStage& stage) const;
    BufferOpenGL(RenderStage stage, size_t size_bytes, size_t offset);
};
}  // namespace Fuego::Graphics
