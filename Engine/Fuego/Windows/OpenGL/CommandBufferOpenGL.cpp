#include "CommandBufferOpenGL.h"

#include "BufferOpenGL.h"
#include "glad/glad.h"

namespace Fuego::Renderer
{
void CommandBufferOpenGL::BindDescriptorSet(std::unique_ptr<DescriptorBuffer> descriptorSet, int setIndex)
{
    UNUSED(descriptorSet);
    UNUSED(setIndex);
}
void CommandBufferOpenGL::BindVertexBuffer(std::unique_ptr<Buffer> vertexBuffer)
{
    const BufferOpenGL* buff = static_cast<const BufferOpenGL*>(vertexBuffer.get());
    glBindBuffer(GL_ARRAY_BUFFER, buff->GetBufferID());
}
CommandBufferOpenGL::CommandBufferOpenGL()
{
}

CommandBufferOpenGL::~CommandBufferOpenGL()
{
}

}  // namespace Fuego::Renderer
