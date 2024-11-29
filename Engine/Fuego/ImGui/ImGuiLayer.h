#pragma once

#include "Layer.h"

namespace Fuego
{
class FUEGO_API ImGuiLayer : public Layer
{
public:
    ImGuiLayer(const std::string& name = "ImGui");
    ~ImGuiLayer() = default;

    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate() override;
    void OnEvent(EventVariant& event) override;
};
}  // namespace Fuego
