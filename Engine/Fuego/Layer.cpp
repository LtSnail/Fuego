#include <Layer.h>

namespace Fuego
{

class Layer::LayerImpl
{
    friend class Layer;

    std::string m_Name;
};

Layer::Layer(const std::string& name)
    : d(new LayerImpl)
{
    d->m_Name = name;
}

Layer::~Layer()
{
    delete d;
}

void Layer::OnAttach()
{
    
}

void Layer::OnDetach()
{
    
}

void Layer::OnUpdate()
{
    
}

void Layer::OnEvent(Event& event)
{
    UNUSED(event);
}

const std::string& Layer::GetName() const
{
    return d->m_Name;
}
}
