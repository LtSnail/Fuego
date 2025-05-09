#pragma once

#include "Application.h"
#include "Buffer.h"
#include "Camera.h"
#include "CommandBuffer.h"
#include "CommandPool.h"
#include "CommandQueue.h"
#include "Device.h"
#include "Material.h"
#include "Model.h"
#include "Services/ServiceInterfaces.hpp"
#include "Shader.h"
#include "ShaderObject.h"
#include "Surface.h"
#include "Swapchain.h"
#include "Texture.h"
#include "VertexLayout.h"
#include "glm/ext.hpp"
#include "glm/glm.hpp"

namespace Fuego::Graphics
{

#pragma pack(push, 1)
struct VertexData
{
    glm::vec3 pos;
    glm::vec2 textcoord;
    glm::vec3 normal;

    VertexData(glm::vec3 pos = glm::vec3(0.0f), glm::vec3 text_coord = glm::vec3(0.0f), glm::vec3 normal = glm::vec3(0.0f));
};
#pragma pack(pop)

class FUEGO_API Renderer : public Service<Renderer, IRendererService, IUpdatable>
{
public:
    struct Viewport
    {
        float width = 0.0f;
        float height = 0.0f;
        float x = 0.0f;
        float y = 0.0f;
    };

    enum TextureType
    {
        ALBEDO = 0,
        DIFFUSE = 1,
        SPECULAR = 2
    };

    friend struct Service<Renderer, IRendererService, IUpdatable>;
    Renderer();
    ~Renderer() = default;

    // IRende;
    void DrawModel(const Model* model, glm::mat4 model_pos);
    void ChangeViewport(float x, float y, float w, float h);
    std::unique_ptr<Texture> CreateTexture(unsigned char* buffer, int width, int height) const;

    // IEngineSubsystem
    void Update(float dlTime) {};
    void PostUpdate(float dlTime) {};

    void Clear();
    void Present();

    void ShowWireFrame();
    void ToggleWireFrame();
    void ValidateWindow();
    inline const Viewport& GetViewport() const
    {
        return viewport;
    }


    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    Renderer(Renderer&&) noexcept = default;
    Renderer& operator=(Renderer&&) noexcept = default;

    static uint32_t MAX_TEXTURES_COUNT;

    inline const ShaderObject* CurrentShaderObject() const
    {
        return current_shader_obj;
    }
    inline void SetShaderObject(ShaderObject* obj)
    {
        current_shader_obj = obj;
    }


    std::unique_ptr<ShaderObject> opaque_shader;

private:
    bool show_wireframe;

    void UpdateViewport();
    std::unique_ptr<Device> _device;
    std::unique_ptr<CommandQueue> _commandQueue;
    std::unique_ptr<CommandPool> _commandPool;
    std::unique_ptr<Swapchain> _swapchain;
    std::unique_ptr<Surface> _surface;
    std::unique_ptr<Camera> _camera;


    ShaderObject* current_shader_obj;

    Viewport viewport;

    // Service
protected:
    void OnInit();
    void OnShutdown();
};
}  // namespace Fuego::Graphics
