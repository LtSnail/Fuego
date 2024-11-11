#pragma once

#include <Layer.h>


namespace Fuego
{
class FUEGO_API LayerStack
{
    FUEGO_INTERFACE(LayerStack)

   public:
    LayerStack();
    ~LayerStack();

    FUEGO_NON_COPYABLE_NON_MOVABLE(LayerStack)

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* overlay);
    void PopLayer(Layer* layer);
    void PopOverlay(Layer* overlay);

    std::vector<Layer*>::iterator begin();
    std::vector<Layer*>::iterator end();
};
}
