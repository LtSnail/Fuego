#pragma once

#include "Application.h"
#include "Buffer.h"
#include "Camera.h"
#include "CommandBuffer.h"
#include "CommandPool.h"
#include "CommandQueue.h"
#include "Device.h"
#include "Material.h"
#include "Mesh.h"
#include "Shader.h"
#include "ShaderObject.h"
#include "Surface.h"
#include "Swapchain.h"
#include "Texture.h"
#include "VertexLayout.h"
#include "glm/ext.hpp"
#include "glm/glm.hpp"

namespace Fuego::Renderer
{

class FUEGO_API Renderer
{
public:
    struct Viewport
    {
        float width = 0.0f;
        float heigth = 0.0f;
        float x = 0.0f;
        float y = 0.0f;
    };

#pragma pack(push, 1)
    struct VertexData
    {
        glm::vec3 pos;
        glm::vec2 textcoord;
        glm::vec3 normal;
    };
#pragma pack(pop)

    enum TextureType
    {
        ALBEDO = 0,
        DIFFUSE = 1,
        SPECULAR = 2
    };


    ~Renderer() = default;
    // TODO replace array of floats to Mesh class
    void DrawMesh(float vertices[], uint32_t vertexCount, uint32_t indices[], uint32_t indicesCount);
    void DrawMesh(const std::vector<float>& data, uint32_t vertex_count, Material* material, glm::mat4 mesh_pos, glm::mat4 camera, glm::mat4 projection);
    void Clear();
    void Present();

    void ShowWireFrame();
    void ToggleWireFrame();
    void ValidateWindow();
    inline const Viewport& GetViewport() const
    {
        return viewport;
    }

    std::unique_ptr<Texture> CreateTexture(unsigned char* buffer, int width, int height) const;

    Renderer(const Renderer&&) = delete;
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    static uint32_t MAX_TEXTURES_COUNT;

private:
    bool show_wireframe;

    void UpdateViewport();
    std::unique_ptr<Buffer> _buffer;
    std::unique_ptr<Device> _device;
    std::unique_ptr<CommandQueue> _commandQueue;
    std::unique_ptr<CommandPool> _commandPool;
    std::unique_ptr<Swapchain> _swapchain;
    std::unique_ptr<Shader> _mainVsShader;
    std::unique_ptr<Shader> _pixelShader;
    std::unique_ptr<Surface> _surface;
    std::unique_ptr<Camera> _camera;

    Viewport viewport;

    friend class Fuego::Application;
    Renderer();
    void ChangeViewport(float x, float y, float w, float h);
};
}  // namespace Fuego::Renderer
