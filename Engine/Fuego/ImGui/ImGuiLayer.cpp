#include "ImGuiLayer.h"

#include <fupch.h>

#include "imgui.h"

namespace Fuego
{

ImGuiLayer::ImGuiLayer(const std::string& name)
    : Layer(name)
{
}

void ImGuiLayer::OnAttach()
{
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    auto io = ImGui::GetIO();
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
}

void ImGuiLayer::OnDetach()
{
}

void ImGuiLayer::OnUpdate(float dtTime)
{
    // TODO
}

void ImGuiLayer::OnEvent(EventVariant& event)
{
    UNUSED(event);
}
}  // namespace Fuego
