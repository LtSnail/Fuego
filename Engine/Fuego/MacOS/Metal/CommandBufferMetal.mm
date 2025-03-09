#include "Metal/CommandBufferMetal.h"
#include "Metal/BufferMetal.h"
#include "Metal/ShaderMetal.h"
#include "Metal/SurfaceMetal.h"
#include "Metal/TextureMetal.h"
#include "Renderer/VertexLayout.h"

#include <QuartzCore/CAMetalLayer.hpp>

namespace Fuego::Renderer
{

CommandBufferMetal::CommandBufferMetal(MTL::CommandBuffer* commandBuffer, MTL::Device* device)
{
    _commandBuffer = commandBuffer;
    _device = device;
}

CommandBufferMetal::~CommandBufferMetal()
{
    _commandBuffer->release();
}

void CommandBufferMetal::BindRenderTarget(const Surface& texture)
{
    _surface = static_cast<const SurfaceMetal*>(&texture);
}

void CommandBufferMetal::BindVertexShader(const Shader& vertexShader)
{
    _vertexShader = static_cast<const ShaderMetal*>(&vertexShader);
}

void CommandBufferMetal::BindPixelShader(const Shader& pixelShader)
{
    _pixelShader = static_cast<const ShaderMetal*>(&pixelShader);
}

void CommandBufferMetal::BindDescriptorSet(const DescriptorBuffer& descriptorSet, int setIndex)
{
}

void CommandBufferMetal::BindVertexBuffer(const Buffer& vertexBuffer, VertexLayout layout)
{
    UNUSED(layout);

    _buffer = static_cast<const BufferMetal*>(&vertexBuffer);
}

void CommandBufferMetal::Draw(uint32_t vertexCount)
{
    CAMetalLayer* layer = (__bridge CAMetalLayer*)_surface->GetNativeHandle();

    auto renderPipelineDescriptor = MTL::RenderPipelineDescriptor::alloc()->init();
    renderPipelineDescriptor->setLabel(NS::String::string("Triangle Rendering Pipeline", NS::ASCIIStringEncoding));
    renderPipelineDescriptor->setVertexFunction(_vertexShader->_function);
    renderPipelineDescriptor->setFragmentFunction(_pixelShader->_function);
    FU_CORE_ASSERT(renderPipelineDescriptor, "Failed to create Render Pipeline Description");
    MTL::PixelFormat pixelFormat = (MTL::PixelFormat)layer.pixelFormat;
    renderPipelineDescriptor->colorAttachments()->object(0)->setPixelFormat(pixelFormat);

    auto vertexDescriptor = MTL::VertexDescriptor::alloc()->init();
    vertexDescriptor->attributes()->object(0)->setFormat(MTL::VertexFormatFloat3);  // position
    vertexDescriptor->attributes()->object(0)->setOffset(0);
    vertexDescriptor->attributes()->object(0)->setBufferIndex(0);

    vertexDescriptor->attributes()->object(1)->setFormat(MTL::VertexFormatFloat3);  // color
    vertexDescriptor->attributes()->object(1)->setOffset(sizeof(float) * 3);
    vertexDescriptor->attributes()->object(1)->setBufferIndex(0);

    vertexDescriptor->layouts()->object(0)->setStride(sizeof(float) * 6);
    vertexDescriptor->layouts()->object(0)->setStepFunction(MTL::VertexStepFunctionPerVertex);

    renderPipelineDescriptor->setVertexDescriptor(vertexDescriptor);
    vertexDescriptor->release();

    NS::Error* error;
    auto metalRenderPSO = _device->newRenderPipelineState(renderPipelineDescriptor, &error);
    FU_CORE_ASSERT(metalRenderPSO, error->localizedDescription()->utf8String())

    renderPipelineDescriptor->release();

    auto metalDrawable = (__bridge CA::MetalDrawable*)[layer nextDrawable];
    FU_CORE_ASSERT(metalDrawable, "Failed to get next drawable from CAMetalLayer");

    MTL::RenderPassDescriptor* renderPassDescriptor = MTL::RenderPassDescriptor::alloc()->init();
    MTL::RenderPassColorAttachmentDescriptor* cd = renderPassDescriptor->colorAttachments()->object(0);
    cd->setTexture(metalDrawable->texture());
    cd->setLoadAction(MTL::LoadActionClear);
    cd->setClearColor(MTL::ClearColor(41.0f / 255.0f, 42.0f / 255.0f, 48.0f / 255.0f, 1.0));
    cd->setStoreAction(MTL::StoreActionStore);

    MTL::RenderCommandEncoder* renderCommandEncoder = _commandBuffer->renderCommandEncoder(renderPassDescriptor);

    renderCommandEncoder->setRenderPipelineState(metalRenderPSO);
    renderCommandEncoder->setVertexBuffer(_buffer->_buffer, 0, 0);
    MTL::PrimitiveType typeTriangle = MTL::PrimitiveTypeTriangle;
    renderCommandEncoder->drawPrimitives(typeTriangle, (NS::UInteger)0, vertexCount);

    renderCommandEncoder->endEncoding();

    _commandBuffer->presentDrawable(metalDrawable);
    _commandBuffer->commit();
    _commandBuffer->waitUntilCompleted();

    renderPassDescriptor->release();
}

}  // namespace Fuego::Renderer
