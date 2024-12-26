#include "CommandBufferOpenGL.h"

#include "BufferOpenGL.h"
#include "Renderer/Surface.h"
#include "ShaderOpenGL.h"
#include "glad/glad.h"

namespace Fuego::Renderer
{

CommandBufferOpenGL::CommandBufferOpenGL()
    : _programID(0)
    , _isFree(true)
    , _vertexShader(-1)
    , _pixelShader(-1)
    , _isLinked(false)
{
    _programID = glCreateProgram();
}

CommandBufferOpenGL::~CommandBufferOpenGL()
{
    glDeleteProgram(_programID);
}

void CommandBufferOpenGL::BeginRecording()
{
    _isFree = false;
}

void CommandBufferOpenGL::EndRecording()
{
    // TODO: add smth
}

void CommandBufferOpenGL::Submit()
{
    // TODO Execute gl commands from commands queue
    _isFree = true;
}

void CommandBufferOpenGL::BindRenderTarget(const Surface& texture)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // TODO: shouldn't be here
    auto handle = (HWND)texture.GetNativeHandle();
    RECT clientRect;
    if (GetClientRect(handle, &clientRect))
    {
        int width = clientRect.right - clientRect.left;
        int height = clientRect.bottom - clientRect.top;

        glViewport(0, 0, width, height);
    }
    else
    {
        FU_CORE_ERROR("[CommandBufferOpenGL] Failed to get client rect for HWND");
    }
}

void CommandBufferOpenGL::BindVertexShader(const Shader& vertexShader)
{
    auto shaderGL = dynamic_cast<const ShaderOpenGL*>(&vertexShader);
    if (!_isLinked)
    {
        glAttachShader(_programID, shaderGL->GetID());
        _vertexShader = shaderGL->GetID();
    }
    else
    {
        if (_vertexShader != shaderGL->GetID())
        {
            glDetachShader(_programID, _vertexShader);
            glAttachShader(_programID, shaderGL->GetID());
            _vertexShader = shaderGL->GetID();
        }
    }
}

void CommandBufferOpenGL::BindPixelShader(const Shader& pixelShader)
{
    auto shaderGL = dynamic_cast<const ShaderOpenGL*>(&pixelShader);
    if (!_isLinked)
    {
        glAttachShader(_programID, shaderGL->GetID());
        _pixelShader = shaderGL->GetID();
    }
    else
    {
        if (_pixelShader != shaderGL->GetID())
        {
            glDetachShader(_programID, _pixelShader);
            glAttachShader(_programID, shaderGL->GetID());
            _pixelShader = shaderGL->GetID();
        }
    }
    // TODO: think how to get rid of the order of binding
    if (_isLinked)
    {
        glUseProgram(_programID);
        return;
    }

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
    _isLinked = true;
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
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

void CommandBufferOpenGL::Clear()
{
    glClearColor(1.f, 1.f, 1.f, 1);
    glClear(GL_COLOR_BUFFER_BIT);
}

void CommandBufferOpenGL::BindDescriptorSet(const DescriptorBuffer& descriptorSet, int setIndex)
{
    UNUSED(descriptorSet);
    UNUSED(setIndex);
    FU_CORE_INFO("[OpenGL unused function: BindDescriptorSet]");
}

}  // namespace Fuego::Renderer
