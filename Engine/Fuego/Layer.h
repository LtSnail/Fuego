#pragma once

#include <Events/Event.h>

#include "Events/EventVisitor.h"

namespace Fuego
{
class FUEGO_API Layer
{
    FUEGO_INTERFACE(Layer)

public:
    Layer(const std::string& name = "Layer");
    virtual ~Layer();

    virtual void OnAttach();
    virtual void OnDetach();
    virtual void OnUpdate();
    virtual void OnEvent(EventVariant& event);

    const std::string& GetName() const;
};
}  // namespace Fuego
