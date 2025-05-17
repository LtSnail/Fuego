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

void Layer::OnUpdate(float dlTime)
{
    // TODO
}

void Layer::OnPostUpdate(float dlTime)
{
    // TODO
}

void Layer::OnFixedUpdate()
{
    // TODO
}

void Layer::OnEvent(EventVariant& event)
{
    UNUSED(event);
}

const std::string& Layer::GetName() const
{
    return d->m_Name;
}
}  // namespace Fuego
