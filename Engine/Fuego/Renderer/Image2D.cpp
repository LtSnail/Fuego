#include "Image2D.h"

#include "Color.h"
#include "Services/ServiceLocator.h"

Fuego::Graphics::Image2D::Image2D(std::string_view name, std::string_view ext, unsigned char* data, int w, int h, int bpp, uint16_t channels)
    : name(name)
    , ext(ext)
    , data(data)
    , width(w)
    , height(h)
    , bpp(bpp)
    , channels(channels)
{
    FU_CORE_ASSERT(bpp > 0 && channels > 0, "Invalid Image data");
    is_created = true;
}
Fuego::Graphics::Image2D::Image2D(std::string_view name, std::string_view ext)
    : name(name)
    , ext(ext)
    , data(nullptr)
    , width(0)
    , height(0)
    , bpp(0)
    , channels(0)
{
    is_created = false;
}

Fuego::Graphics::Image2D::~Image2D()
{
    ServiceLocator::instance().GetService<Fuego::AssetsManager>()->FreeImage2D(data);
}

void Fuego::Graphics::Image2D::PostCreate(Image2DPostCreateion& settings)
{
    FU_CORE_ASSERT(settings.data, "[Image2D] data is nullptr");
    width = settings.width;
    height = settings.height;
    bpp = settings.bpp;
    channels = settings.channels;
    data = settings.data;
    is_created = true;
}
