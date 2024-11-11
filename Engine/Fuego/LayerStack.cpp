#include <LayerStack.h>

namespace Fuego
{

class LayerStack::LayerStackImpl
{
    friend class LayerStack;

    std::vector<Layer*> m_Layers;
    std::vector<Layer*>::iterator m_LayerInsert;
};

LayerStack::LayerStack() : d(new LayerStackImpl)
{
    d->m_LayerInsert = d->m_Layers.begin();
}

LayerStack::~LayerStack()
{
    for (auto layer : d->m_Layers)
    {
        delete layer;
    }

    delete d;
}

void LayerStack::PushLayer(Layer* layer)
{
    d->m_LayerInsert = d->m_Layers.emplace(d->m_LayerInsert, layer);
}

void LayerStack::PushOverlay(Layer* overlay)
{
    d->m_Layers.emplace_back(overlay);
}

void LayerStack::PopLayer(Layer* layer)
{
    auto it = std::ranges::find(d->m_Layers.begin(), d->m_Layers.end(), layer);
    if (it != d->m_Layers.end())
    {
        d->m_Layers.erase(it);
        --d->m_LayerInsert;
    }
}

void LayerStack::PopOverlay(Layer* overlay)
{
    auto it =
        std::ranges::find(d->m_Layers.begin(), d->m_Layers.end(), overlay);
    if (it != d->m_Layers.end())
    {
        d->m_Layers.erase(it);
    }
}

std::vector<Layer*>::iterator LayerStack::begin()
{
    return d->m_Layers.begin();
}

std::vector<Layer*>::iterator LayerStack::end()
{
    return d->m_Layers.end();
}
}
