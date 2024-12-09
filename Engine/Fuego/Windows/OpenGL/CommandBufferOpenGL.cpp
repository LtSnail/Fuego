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

void CommandBufferOpenGL::BindRenderTarget(const Surface& texture)
{
    UNUSED(texture);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glViewport(0, 0, viewport[2], viewport[3]);
}

void CommandBufferOpenGL::BindVertexShader(const Shader& vertexShader)
{
    auto shaderGL = dynamic_cast<const ShaderOpenGL*>(&vertexShader);
    glAttachShader(_programID, shaderGL->GetID());
}

void CommandBufferOpenGL::BindPixelShader(const Shader& pixelShader)
{
    auto shaderGL = dynamic_cast<const ShaderOpenGL*>(&pixelShader);
    glAttachShader(_programID, shaderGL->GetID());

    // TODO: think how to get rid of the order of binding
    glLinkProgram(_programID);

    GLint success;
    glGetProgramiv(_programID, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(_programID, 512, nullptr, infoLog);
        FU_CORE_ERROR("[CommandBufferOpenGL] shader linking error: ", infoLog);
        return;
    }

    glUseProgram(_programID);
}

void CommandBufferOpenGL::BindVertexBuffer(const Buffer& vertexBuffer)
{
    const BufferOpenGL& buff = static_cast<const BufferOpenGL&>(vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, buff.GetBufferID());

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);  // Position attribute
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(3 * sizeof(float)));  // Color attribute
    glEnableVertexAttribArray(1);
}

void CommandBufferOpenGL::Draw(uint32_t vertexCount)
{
    // glDrawArrays(GL_TRIANGLES, 0, vertexCount); // TODO: Uncomment
}

void CommandBufferOpenGL::BindDescriptorSet(const DescriptorBuffer& descriptorSet, int setIndex)
{
    UNUSED(descriptorSet);
    UNUSED(setIndex);
    FU_CORE_INFO("[OpenGL unused function: BindDescriptorSet]");
}

}  // namespace Fuego::Renderer
