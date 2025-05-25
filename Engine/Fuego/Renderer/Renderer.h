#pragma once

#include "Application.h"
#include "Buffer.h"
#include "Camera.h"
#include "CommandBuffer.h"
#include "CommandPool.h"
#include "CommandQueue.h"
#include "Device.h"
#include "Graphics.hpp"
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

class Image2D
{
public:
    Image2D(std::string name, unsigned char* data, int w, int h, int bpp, uint16_t channels)
        : name(name)
        , data(data)
        , width(w)
        , height(h)
        , bpp(bpp)
        , channels(channels)
    {
        FU_CORE_ASSERT(bpp > 0 && channels > 0, "Invalid Image data");
    }
    ~Image2D()
    {
        delete data;
    }

    inline uint32_t Width() const
    {
        return width;
    }
    inline uint32_t Height() const
    {
        return height;
    }
    inline uint16_t BBP() const
    {
        return bpp;
    }
    inline uint16_t Channels() const
    {
        return channels;
    }
    unsigned char* Data() const
    {
        return data;
    }
    inline std::string_view Name() const
    {
        return name;
    }

private:
    uint32_t width;
    uint32_t height;
    uint16_t bpp;
    uint16_t channels;
    unsigned char* data;
    std::string name;
};

class FUEGO_API Renderer : public Service<Renderer>, public IUpdatable
{
public:
    friend struct Service<Renderer>;

    Renderer(GraphicsAPI api);
    ~Renderer() = default;

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    Renderer(Renderer&&) noexcept = default;
    Renderer& operator=(Renderer&&) noexcept = default;

    const Texture* CreateTexture(const Image2D& img);
    const Texture* GetLoadedTexture(std::string_view name) const;

    // IRenderer;
    void DrawModel(const Model* model, glm::mat4 model_pos);
    void ChangeViewport(float x, float y, float w, float h);

    // IUpdatable
    void OnUpdate(float dlTime);
    void OnPostUpdate(float dlTime);
    void OnFixedUpdate();

    void Clear();
    void Present();

    void ShowWireFrame();
    void ToggleWireFrame();
    void ValidateWindow();
    inline const Viewport& GetViewport() const
    {
        return viewport;
    }

    void SetVSync(bool active);
    bool IsVSync();

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

    bool is_vsync;

    std::unordered_map<std::string, std::unique_ptr<Texture>> textures;
    GraphicsAPI renderer;
    // Service
protected:
    void OnInit();
    void OnShutdown();
};
}  // namespace Fuego::Graphics
