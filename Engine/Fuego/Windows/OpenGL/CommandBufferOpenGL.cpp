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

CommandBufferOpenGL::~CommandBufferOpenGL()
{
    glDeleteProgram(_programID);
}

void CommandBufferOpenGL::BindRenderTarget(std::unique_ptr<Texture> texture)
{
    UNUSED(texture);
}

void CommandBufferOpenGL::BindVertexShader(std::unique_ptr<Shader> vertexShader)
{
    ShaderOpenGL* shaderGL = static_cast<ShaderOpenGL*>(vertexShader.get());
    glAttachShader(_programID, shaderGL->GetID());
}

void CommandBufferOpenGL::BindPixelShader(std::unique_ptr<Shader> pixelShader)
{
    ShaderOpenGL* shaderGL = static_cast<ShaderOpenGL*>(pixelShader.get());
    glAttachShader(_programID, shaderGL->GetID());
}

void CommandBufferOpenGL::BindVertexBuffer(std::unique_ptr<Buffer> vertexBuffer)
{
    const BufferOpenGL* buff = static_cast<const BufferOpenGL*>(vertexBuffer.get());
    glBindBuffer(GL_ARRAY_BUFFER, buff->GetBufferID());
}

void CommandBufferOpenGL::Draw(uint32_t vertexCount)
{
    glLinkProgram(_programID);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

void CommandBufferOpenGL::BindDescriptorSet(std::unique_ptr<DescriptorBuffer> descriptorSet, int setIndex)
{
    UNUSED(descriptorSet);
    UNUSED(setIndex);
    FU_CORE_INFO("[OpenGL unused function: BindDescriptorSet]");
}

}  // namespace Fuego::Renderer
