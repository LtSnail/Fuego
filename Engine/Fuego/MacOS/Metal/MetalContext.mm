#include "MetalContext.h"

#include <QuartzCore/CAMetalLayer.hpp>

namespace Fuego
{

MetalContext::MetalContext(void* nsWindow)
    : _window(nsWindow)
{
    _device = MTL::CreateSystemDefaultDevice();
    FU_CORE_ASSERT(_window, "NSWindow is nullptr");
    FU_CORE_ASSERT(_device, "Metal device is nullptr");
}

MetalContext::~MetalContext()
{
    if (_metalLayer)
    {
        CFRelease(_metalLayer);
        _metalLayer = nullptr;
    }
    _device->release();
}

bool MetalContext::Init()
{    
    CAMetalLayer* metalLayer = [CAMetalLayer layer];
    metalLayer.device = (__bridge id<MTLDevice>)_device;
    metalLayer.pixelFormat = MTLPixelFormatBGRA8Unorm;
    
    _metalLayer = (__bridge_retained void*)metalLayer;
    
    NSWindow* w = (__bridge NSWindow*)_window;
    w.contentView.layer = metalLayer;
    w.contentView.wantsLayer = YES;
    
    // Temporary code to draw a triangle as soon as possible.
    simd::float3 triangleVertices[] = {
            {-0.5f, -0.5f, 0.0f},
            { 0.5f, -0.5f, 0.0f},
            { 0.0f,  0.5f, 0.0f}
        };

    triangleVertexBuffer = _device->newBuffer(&triangleVertices,
                                              sizeof(triangleVertices),
                                              MTL::ResourceStorageModeShared);
    
    MTL::Library* metalDefaultLibrary = _device->newDefaultLibrary();
    FU_CORE_ASSERT(metalDefaultLibrary, "Failed to load default library");
    
    metalCommandQueue = _device->newCommandQueue();
    
    MTL::Function* vertexShader = metalDefaultLibrary->newFunction(NS::String::string("vertexShader", NS::ASCIIStringEncoding));
    FU_CORE_ASSERT(vertexShader, "Failed to create vertex shader");
    MTL::Function* fragmentShader = metalDefaultLibrary->newFunction(NS::String::string("fragmentShader", NS::ASCIIStringEncoding));
    FU_CORE_ASSERT(fragmentShader, "Failed to create fragment shader");

    MTL::RenderPipelineDescriptor* renderPipelineDescriptor = MTL::RenderPipelineDescriptor::alloc()->init();
    renderPipelineDescriptor->setLabel(NS::String::string("Triangle Rendering Pipeline", NS::ASCIIStringEncoding));
    renderPipelineDescriptor->setVertexFunction(vertexShader);
    renderPipelineDescriptor->setFragmentFunction(fragmentShader);
    FU_CORE_ASSERT(renderPipelineDescriptor, "Failed to create Render Pipeline Description");
    MTL::PixelFormat pixelFormat = (MTL::PixelFormat)metalLayer.pixelFormat;
    renderPipelineDescriptor->colorAttachments()->object(0)->setPixelFormat(pixelFormat);

    NS::Error* error;
    metalRenderPSO = _device->newRenderPipelineState(renderPipelineDescriptor, &error);

    renderPipelineDescriptor->release();

    return true;
}

void MetalContext::SwapBuffers()
{
    CAMetalLayer* layer = (__bridge CAMetalLayer*)_metalLayer;
    _metalDrawable = (__bridge CA::MetalDrawable*)[layer nextDrawable];
    FU_CORE_ASSERT(_metalDrawable, "Failed to get next drawable from CAMetalLayer");

    MTL::CommandBuffer* metalCommandBuffer = metalCommandQueue->commandBuffer();

    MTL::RenderPassDescriptor* renderPassDescriptor = MTL::RenderPassDescriptor::alloc()->init();
    MTL::RenderPassColorAttachmentDescriptor* cd = renderPassDescriptor->colorAttachments()->object(0);
    cd->setTexture(_metalDrawable->texture());
    cd->setLoadAction(MTL::LoadActionClear);
    cd->setClearColor(MTL::ClearColor(41.0f/255.0f, 42.0f/255.0f, 48.0f/255.0f, 1.0));
    cd->setStoreAction(MTL::StoreActionStore);

    MTL::RenderCommandEncoder* renderCommandEncoder = metalCommandBuffer->renderCommandEncoder(renderPassDescriptor);
    
    renderCommandEncoder->setRenderPipelineState(metalRenderPSO);
    renderCommandEncoder->setVertexBuffer(triangleVertexBuffer, 0, 0);
    MTL::PrimitiveType typeTriangle = MTL::PrimitiveTypeTriangle;
    NS::UInteger vertexStart = 0;
    NS::UInteger vertexCount = 3;
    renderCommandEncoder->drawPrimitives(typeTriangle, vertexStart, vertexCount);
    
    renderCommandEncoder->endEncoding();

    metalCommandBuffer->presentDrawable(_metalDrawable);
    metalCommandBuffer->commit();
    metalCommandBuffer->waitUntilCompleted();

    renderPassDescriptor->release();
}
}  // namespace Fuego
