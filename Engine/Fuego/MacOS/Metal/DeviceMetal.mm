#include "Metal/DeviceMetal.h"
#include "Renderer/Device.h"
#include "Metal/BufferMetal.h"
#include "Metal/ShaderMetal.h"
#include "Metal/CommandQueueMetal.h"

#include <QuartzCore/CAMetalLayer.hpp>
#include <QuartzCore/QuartzCore.hpp>


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

std::unique_ptr<CommandQueue> DeviceMetal::CreateQueue()
{
    return std::make_unique<CommandQueueMetal>(_device->newCommandQueue());
}

std::unique_ptr<CommandPool> DeviceMetal::CreateCommandPool(const CommandQueue& queue)
{
    UNUSED(queue);

    return nullptr;
}

std::unique_ptr<Swapchain> DeviceMetal::CreateSwapchain(const Surface& surface)
{
    UNUSED(surface);

    return nullptr;
}

std::unique_ptr<Shader> DeviceMetal::CreateShader(std::string_view shaderName)
{
    auto shader = _defaultLibrary->newFunction(NS::String::string(shaderName.data(), NS::ASCIIStringEncoding));
    FU_CORE_ASSERT(shader, "Failed to create shader");
    
    return std::make_unique<ShaderMetal>(shader);
}

std::unique_ptr<Device> Device::CreateDevice(const Surface& surface)
{
    UNUSED(surface);
    
    return std::make_unique<DeviceMetal>();
}
}  // namespace Fuego::Renderer
