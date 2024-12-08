#include "CommandBufferOpenGL.h"

#include "BufferOpenGL.h"
#include "ShaderOpenGL.h"
#include "glad/glad.h"

namespace Fuego::Renderer
{

CommandBufferOpenGL::CommandBufferOpenGL()
    : _programID(0)
{
    _programID = glCreateProgram();
}

std::unique_ptr<CommandBuffer> CommandBuffer::CreateCommandBuffer()
{
    return std::make_unique<CommandBufferOpenGL>();
}

CommandBufferOpenGL::~CommandBufferOpenGL()
{
    glDeleteProgram(_programID);
}

void CommandBufferOpenGL::BindRenderTarget(const Texture& texture)
{
    UNUSED(texture);
}

void CommandBufferOpenGL::BindVertexShader(const Shader& vertexShader)
{
    const ShaderOpenGL& shaderGL = static_cast<const ShaderOpenGL&>(vertexShader);
    glAttachShader(_programID, shaderGL.GetID());
}

void CommandBufferOpenGL::BindPixelShader(const Shader& pixelShader)
{
    const ShaderOpenGL& shaderGL = static_cast<const ShaderOpenGL&>(pixelShader);
    glAttachShader(_programID, shaderGL.GetID());
}

void CommandBufferOpenGL::BindVertexBuffer(const Buffer& vertexBuffer)
{
    const BufferOpenGL& buff = static_cast<const BufferOpenGL&>(vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, buff.GetBufferID());
}

void CommandBufferOpenGL::Draw(uint32_t vertexCount)
{
    glLinkProgram(_programID);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

void CommandBufferOpenGL::BindDescriptorSet(const DescriptorBuffer& descriptorSet, int setIndex)
{
    UNUSED(descriptorSet);
    UNUSED(setIndex);
    FU_CORE_INFO("[OpenGL unused function: BindDescriptorSet]");
}

}  // namespace Fuego::Renderer
