#pragma once

#include "Renderer/Buffer.h"

namespace Fuego::Renderer
{
class BufferOpenGL : public Buffer
{
public:
    virtual ~BufferOpenGL() override;

    uint32_t GetBufferID() const;
    virtual void BindDataImpl(const void* data, size_t size, size_t offset) override;

private:
    uint32_t _vbo;

protected:
    friend class DeviceOpenGL;
    BufferOpenGL();
};
}  // namespace Fuego::Renderer
