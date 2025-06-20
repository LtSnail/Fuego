#include "CommandBufferOpenGL.h"

#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include "../../Renderer/Graphics.hpp"
#include "BufferOpenGL.h"
#include "Renderer.h"
#include "ShaderObjectOpenGL.h"
#include "TextureOpenGL.h"
#include "VertexLayout.h"
#include "glad/gl.h"

namespace Fuego::Graphics
{
CommandBufferOpenGL::CommandBufferOpenGL()
    : _mainVsShader(-1)
    , _pixelShader(-1)
    , _isLinked(false)
    , _isDataAllocated(false)
    , _texture(0)
    , _isFree(true)
{
    glCreateVertexArrays(1, &_vao);
}

CommandBufferOpenGL::~CommandBufferOpenGL()
{
    glDeleteVertexArrays(1, &_vao);

    for (size_t i = 0; i < push_debug_group_commands; i++)
    {
        PopDebugGroup();
    }
}

void CommandBufferOpenGL::BeginRecording()
{
    glBindVertexArray(_vao);
    _isFree = false;
}

void CommandBufferOpenGL::EndRecording()
{
}

void CommandBufferOpenGL::Submit()
{
    _isFree = true;
}

void CommandBufferOpenGL::BindRenderTarget(const Surface& texture)
{
    // glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CommandBufferOpenGL::BindVertexBuffer(std::unique_ptr<Buffer> vertexBuffer, VertexLayout layout)
{
    vertex_global_buffer = std::move(vertexBuffer);
    auto buff = static_cast<const BufferOpenGL*>(vertex_global_buffer.get());

    glVertexArrayVertexBuffer(_vao, 0, buff->GetBufferID(), 0, sizeof(VertexData));

    VertexLayout::LayoutIterator* it;
    for (it = layout.GetIteratorBegin(); it && !it->IsDone(); it = layout.GetNextIterator())
    {
        GLuint index = static_cast<GLuint>(it->GetIndex());
        glVertexArrayAttribFormat(_vao, index, it->GetComponentsAmount(), it->GetAPIDatatype(), GL_FALSE, static_cast<GLuint>(it->GetOffset()));
        glVertexArrayAttribBinding(_vao, index, 0);
        if (it->GetIsEnabled())
            glEnableVertexArrayAttrib(_vao, index);
    }
}

void CommandBufferOpenGL::BindIndexBuffer(std::unique_ptr<Buffer> buffer)
{
    index_global_buffer = std::move(buffer);
    auto buff = static_cast<const BufferOpenGL*>(index_global_buffer.get());

    glVertexArrayElementBuffer(_vao, buff->GetBufferID());
}

uint32_t CommandBufferOpenGL::UpdateBufferSubDataImpl(Buffer::BufferType type, const void* data, size_t size_bytes)
{
    if (type == Buffer::BufferType::Vertex)
        return vertex_global_buffer->UpdateSubData(data, size_bytes);
    else
        return index_global_buffer->UpdateSubData(data, size_bytes);
}

void CommandBufferOpenGL::BindTexture(Texture* texture)
{
    TextureOpenGL& text_gl = static_cast<TextureOpenGL&>(*texture);

    glBindTextureUnit(text_gl.GetTextureUnit(), text_gl.GetTextureID());
}

void CommandBufferOpenGL::Draw(uint32_t vertexCount)
{
    glBindVertexArray(_vao);

    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glBindVertexArray(0);
}

void CommandBufferOpenGL::IndexedDraw(uint32_t index_count, size_t index_offset_bytes, uint32_t base_vertex)
{
    glDrawElementsBaseVertex(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, reinterpret_cast<void*>(index_offset_bytes), base_vertex);
}

void CommandBufferOpenGL::Clear()
{
    glClearColor(1.f, 1.f, 1.f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CommandBufferOpenGL::PushDebugGroup(uint32_t id, const char* message)
{
    if (!message || !*message || *message == '\n')
    {
        FU_CORE_INFO("[Render Marker] PushDebugGroup: message is empty or invalid");
        return;
    }

    GLsizei length = static_cast<GLsizei>(std::strlen(message));
    glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, id, length, message);

    push_debug_group_commands++;
}

void CommandBufferOpenGL::PopDebugGroup()
{
    glPopDebugGroup();
    push_debug_group_commands--;
}

void CommandBufferOpenGL::SetLabel(ObjectLabel id, uint32_t name, const char* message)
{
    if (!message || !*message || *message == '\n')
    {
        FU_CORE_INFO("[Render Marker] SetLabel: message is empty or invalid");
        return;
    }

    const GLsizei length = static_cast<GLsizei>(std::strlen(message));

    GLenum identifier = GL_BUFFER;
    switch (id)
    {
    case Fuego::Graphics::CommandBuffer::LABEL_BUFFER:
        identifier = GL_BUFFER;
        break;
    case Fuego::Graphics::CommandBuffer::LABEL_SHADER:
        identifier = GL_SHADER;
        break;
    case Fuego::Graphics::CommandBuffer::LABEL_TEXTURE:
        identifier = GL_TEXTURE;
        break;
    default:
        FU_CORE_WARN("SetLabel: Unknown object label type");
        return;
    }

    glObjectLabel(identifier, name, length, message);
}

void CommandBufferOpenGL::BindShaderObject(std::shared_ptr<Fuego::Graphics::ShaderObject> shader)
{
    shader_object = shader;
}

void CommandBufferOpenGL::BindDescriptorSet(const DescriptorBuffer& descriptorSet, int setIndex)
{
    UNUSED(descriptorSet);
    UNUSED(setIndex);
    FU_CORE_INFO("[OpenGL unused function: BindDescriptorSet]");
}

int CommandBufferOpenGL::ConvertUsage(RenderStage& stage) const
{
    switch (stage)
    {
    case STATIC_GEOMETRY:
        return GL_STATIC_DRAW;
    case DYNAMIC_DRAW:
        return GL_DYNAMIC_DRAW;
    }
}

}  // namespace Fuego::Graphics
