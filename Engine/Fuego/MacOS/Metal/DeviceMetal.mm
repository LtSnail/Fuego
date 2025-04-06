#include "Metal/DeviceMetal.h"
#include "Metal/BufferMetal.h"
#include "Metal/CommandPoolMetal.h"
#include "Metal/CommandQueueMetal.h"
#include "Metal/ShaderMetal.h"
#include "Metal/SurfaceMetal.h"
#include "Metal/SwapchainMetal.h"
#include "Renderer/Device.h"

#include <QuartzCore/CAMetalLayer.hpp>
#include <QuartzCore/QuartzCore.hpp>

#include "Material.h"
#include "ShaderObject.h"


namespace Fuego::Renderer
{
DeviceMetal::DeviceMetal()
{
    _device = MTL::CreateSystemDefaultDevice();
    FU_CORE_ASSERT(_device, "Failed to create device.");

    _defaultLibrary = _device->newDefaultLibrary();
    FU_CORE_ASSERT(_defaultLibrary, "Failed to load default library");
}

DeviceMetal::~DeviceMetal()
{
    _device->release();
}

std::unique_ptr<Buffer> DeviceMetal::CreateBuffer(size_t size, uint32_t flags)
{
    UNUSED(flags);

    return std::make_unique<BufferMetal>(_device->newBuffer(size, MTL::ResourceStorageModeShared));
}

std::unique_ptr<CommandQueue> DeviceMetal::CreateCommandQueue()
{
    return std::make_unique<CommandQueueMetal>(_device->newCommandQueue());
}

std::unique_ptr<CommandPool> DeviceMetal::CreateCommandPool(const CommandQueue& queue)
{
    auto* commandQueue = dynamic_cast<const CommandQueueMetal*>(&queue)->_commandQueue;
    return std::make_unique<CommandPoolMetal>(commandQueue, _device);
}

std::unique_ptr<Swapchain> DeviceMetal::CreateSwapchain(const Surface& surface)
{
    return std::make_unique<SwapchainMetal>(surface);
}

std::unique_ptr<Shader> DeviceMetal::CreateShader(std::string_view shaderName, Shader::ShaderType shaderType)
{
    UNUSED(shaderType);
    auto shader = _defaultLibrary->newFunction(NS::String::string(shaderName.data(), NS::ASCIIStringEncoding));
    FU_CORE_ASSERT(shader, "Failed to create shader");

    return std::make_unique<ShaderMetal>(shader);
}

std::unique_ptr<Surface> DeviceMetal::CreateSurface(const void* window)
{
    return std::make_unique<SurfaceMetal>(window, _device);
}

std::unique_ptr<Texture> DeviceMetal::CreateTexture(unsigned char* buffer, int width, int height)
{
    return std::make_unique<TextureMetal>();
}

std::unique_ptr<Device> Device::CreateDevice()
{
    return std::make_unique<DeviceMetal>();
}

// TODO: to remove from here.
Material* Material::CreateMaterial(Texture* albedo)
{
    return nullptr;
}
ShaderObject* ShaderObject::CreateShaderObject(Shader& vs, Shader& px)
{
    return nullptr;
}

}  // namespace Fuego::Renderer
