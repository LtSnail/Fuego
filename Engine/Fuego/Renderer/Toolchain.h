#pragma once
#include <memory>
#include <mutex>

#include "Device.h"
#include "Image2D.h"
#include "Texture.h"

using img_text_pair = std::pair<std::shared_ptr<Fuego::Graphics::Image2D>, std::shared_ptr<Fuego::Graphics::Texture>>;

namespace Fuego
{

struct IRendererToolchain
{
    virtual ~IRendererToolchain() = default;
    virtual std::shared_ptr<Fuego::Graphics::Texture> LoadTexture(std::shared_ptr<Fuego::Graphics::Image2D> img, const Fuego::Graphics::Device* device) = 0;
    virtual void Update() = 0;
};

class PostLoadToolchain : public IRendererToolchain
{
public:
    PostLoadToolchain() = default;
    virtual ~PostLoadToolchain() override = default;

    std::shared_ptr<Fuego::Graphics::Texture> LoadTexture(std::shared_ptr<Fuego::Graphics::Image2D> img, const Fuego::Graphics::Device* device) override
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return pairs_.emplace_back(std::make_pair(img, device->CreateTexture(img->Name()))).second;
    }

    void Update() override
    {
        std::lock_guard<std::mutex> lock(mutex_);
        for (auto it = pairs_.begin(); it != pairs_.end();)
        {
            if (it->first->IsValid())
            {
                it->second->PostCreate(it->first);
                it = pairs_.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

private:
    std::list<img_text_pair> pairs_;
    std::mutex mutex_;
};

}  // namespace Fuego
