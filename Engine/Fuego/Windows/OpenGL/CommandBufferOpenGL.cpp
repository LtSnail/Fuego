#include "CommandBufferOpenGL.h"

#include "BufferOpenGL.h"
#include "ShaderOpenGL.h"
#include "glad/glad.h"

namespace Fuego::Renderer
{

CommandBufferOpenGL::CommandBufferOpenGL()
    : _cmdBuf()
{
    _cmdBuf.reserve(10);
}

CommandBufferOpenGL::~CommandBufferOpenGL()
{
}

void CommandBufferOpenGL::BindPixelShader(std::unique_ptr<Shader> pixelShader)
{
    ShaderOpenGL* shader = static_cast<ShaderOpenGL*>(pixelShader.get());
    auto func = &ShaderOpenGL::BindPixelShader;
    _cmdBuf.push_back(
        [shader, func, ]()
        {

        });
}

void CommandBufferOpenGL::BindVertexBuffer(std::unique_ptr<Buffer> vertexBuffer)
{
    _cmdBuf.push_back(
        [_vb = vertexBuffer.get()](void*)
        {
            const BufferOpenGL* buff = static_cast<const BufferOpenGL*>(_vb);
            glBindBuffer(GL_ARRAY_BUFFER, buff->GetBufferID());
        });
}

void CommandBufferOpenGL::Draw(uint32_t vertexCount)
{
    _cmdBuf.push_back([vertexCount](void*) { glDrawArrays(GL_TRIANGLES, 0, vertexCount); });
}

void CommandBufferOpenGL::BindDescriptorSet(std::unique_ptr<DescriptorBuffer> descriptorSet, int setIndex)
{
    UNUSED(descriptorSet);
    UNUSED(setIndex);
    FU_CORE_INFO("[OpenGL unused function: BindDescriptorSet]");
}

}  // namespace Fuego::Renderer
