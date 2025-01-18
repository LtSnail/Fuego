#include "CommandBufferOpenGL.h"

#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include "BufferOpenGL.h"
#include "Renderer.h"
#include "Renderer/Surface.h"
#include "ShaderOpenGL.h"
#include "glad/gl.h"

namespace Fuego::Renderer
{
glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1280.0F / 720.0F, 0.1f, 100.0f);
int modelLoc;
int viewLoc;
int projLoc;

CommandBufferOpenGL::CommandBufferOpenGL()
    : _programID(0)
    , _isFree(true)
    , _vertexShader(-1)
    , _pixelShader(-1)
    , _isLinked(false)
    , _vao(0)
    , _ebo(0)
    , _isDataAllocated(false)
{
    _programID = glCreateProgram();
    glGenBuffers(1, &_ebo);
    glGenVertexArrays(1, &_vao);
}

CommandBufferOpenGL::~CommandBufferOpenGL()
{
    glDeleteProgram(_programID);
    glDeleteBuffers(1, &_vao);
    glDeleteBuffers(1, &_ebo);
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
    // glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
            glDeleteShader(_vertexShader);
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

        glLinkProgram(_programID);
        GLint success;
        glGetProgramiv(_programID, GL_LINK_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetProgramInfoLog(_programID, 512, nullptr, infoLog);
            FU_CORE_ERROR("[CommandBufferOpenGL] shader linking error: ", infoLog);
            _isLinked = false;
            return;
        }
        _isLinked = true;

        glUseProgram(_programID);
        modelLoc = glGetUniformLocation(_programID, "model");
        viewLoc = glGetUniformLocation(_programID, "view");
        projLoc = glGetUniformLocation(_programID, "projection");
        glUseProgram(0);

        return;
    }
    else
    {
        if (_pixelShader != shaderGL->GetID())
        {
            glDetachShader(_programID, _pixelShader);
            glDeleteShader(_pixelShader);
            glAttachShader(_programID, shaderGL->GetID());
            _pixelShader = shaderGL->GetID();
        }
        else
        {
            model = glm::rotate(model, glm::radians(0.5f), glm::vec3(1.1f, 1.0f, 1.0f));
        }
    }
}

void CommandBufferOpenGL::BindVertexBuffer(const Buffer& vertexBuffer)
{
    glBindVertexArray(_vao);

    const BufferOpenGL& buff = static_cast<const BufferOpenGL&>(vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, buff.GetBufferID());

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Renderer::VertexData), (void*)offsetof(Renderer::VertexData, pos));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Renderer::VertexData), (void*)offsetof(Renderer::VertexData, textcoord));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Renderer::VertexData), (void*)offsetof(Renderer::VertexData, normal));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CommandBufferOpenGL::BindIndexBuffer(uint32_t indices[], uint32_t size)
{
    glBindVertexArray(_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    if (!_isDataAllocated)
    {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_DYNAMIC_DRAW);
        _isDataAllocated = true;
    }
    else
    {
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, indices);
    }
    glBindVertexArray(0);
}

void CommandBufferOpenGL::Draw(uint32_t vertexCount)
{
    glUseProgram(_programID);
    glBindVertexArray(_vao);

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(Renderer::GetActiveCamera()->GetView()));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glBindVertexArray(0);
}

void CommandBufferOpenGL::IndexedDraw(uint32_t vertexCount)
{
    glUseProgram(_programID);
    glBindVertexArray(_vao);

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(Renderer::GetActiveCamera()->GetView()));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

void CommandBufferOpenGL::Clear()
{
    glClearColor(1.f, 1.f, 1.f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CommandBufferOpenGL::BindDescriptorSet(const DescriptorBuffer& descriptorSet, int setIndex)
{
    UNUSED(descriptorSet);
    UNUSED(setIndex);
    FU_CORE_INFO("[OpenGL unused function: BindDescriptorSet]");
}

}  // namespace Fuego::Renderer
