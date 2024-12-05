#pragma once

namespace Fuego::Engine
{
class Buffer;
class Texture;
class Sampler;

class DescriptorBuffer
{
public:
    virtual ~DescriptorBuffer() = default;

    virtual void AddBuffer(const Buffer& buffer, uint32_t bindingIndex, size_t offset = 0) = 0;
    virtual void AddTexture(const Texture& texture, uint32_t bindingIndex) = 0;
    virtual void AddSampler(const Sampler& sampler, uint32_t bindingIndex) = 0;
};

}  // namespace Fuego::Engine
