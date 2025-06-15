#pragma once

#include "Device.h"
#include "Image2D.h"
#include "Texture.h"

namespace Fuego::Graphics
{
class Texture;
struct TexturePostCreation;
}  // namespace Fuego::Graphics

namespace Fuego::Pipeline
{
using img_text_pair = std::pair<std::shared_ptr<Fuego::Graphics::Image2D>, std::shared_ptr<Fuego::Graphics::Texture>>;

struct Toolchain
{
    struct renderer
    {
        std::shared_ptr<Fuego::Graphics::Texture> (*load_texture)(std::shared_ptr<Fuego::Graphics::Image2D> img,
                                                                  const Fuego::Graphics::Device* device){nullptr};
        void (*update)(){nullptr};
        static std::list<img_text_pair> pairs;
    };
    struct assets_manager
    {
        // void (*update)(){nullptr};
    };

    renderer _renderer;
    assets_manager _assets_manager;
};

struct PostLoadPipeline
{
    static std::shared_ptr<Fuego::Graphics::Texture> load_texture(std::shared_ptr<Fuego::Graphics::Image2D> img, const Fuego::Graphics::Device* device)
    {
        std::mutex mx;
        std::lock_guard<std::mutex> lock(mx);
        return pairs_ptr->emplace_back(std::make_pair(img, device->CreateTexture(img->Name()))).second;
    }

    static void update()
    {
        std::mutex mx;
        std::lock_guard<std::mutex> lock(mx);

        if (pairs_ptr->empty())
            return;

        for (auto it = pairs_ptr->begin(); it != pairs_ptr->end();)
        {
            if (it->first->IsValid())
            {
                it->second->PostCreate(it->first);
                it = pairs_ptr->erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    static std::list<img_text_pair>* pairs_ptr;
};
}  // namespace Fuego::Pipeline