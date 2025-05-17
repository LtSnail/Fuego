#pragma once

#include <Events/Event.h>

#include "Events/EventVisitor.h"

namespace Fuego
{
class FUEGO_API Layer : IUpdatable
{
    FUEGO_INTERFACE(Layer)

public:
    Layer(const std::string& name = "Layer");
    virtual ~Layer();

    virtual void OnAttach();
    virtual void OnDetach();

    virtual void OnUpdate(float dlTime);
    virtual void OnPostUpdate(float dlTime);
    virtual void OnFixedUpdate();

    virtual void OnEvent(EventVariant& event);

    const std::string& GetName() const;
};
}  // namespace Fuego
