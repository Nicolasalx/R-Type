/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** draw_ping
*/

#include "systems/draw_ping.hpp"
#include <array>
#include <format>
#include "../../utils/Logger.hpp"
#include "components/ping.hpp"
#include "imgui.h"

void ecs::systems::drawPing(Registry &reg, const sf::Vector2u &windowSize)
{
    static constexpr std::size_t MAX_HISTORY_SIZE = 100;
    static std::array<float, MAX_HISTORY_SIZE> pingArray = {0};
    static std::size_t pingIndex = 0;
    static std::size_t pingCount = 0;
    double ping = 0;
    auto pingComponent = reg.getComponents<ecs::component::Ping>().begin();

    if (pingComponent->has_value()) {
        ping = pingComponent->value().ping;
    } else {
        eng::logTimeWarning("Ping component not found.");
    }
    pingArray.at(pingIndex) = ping;
    pingIndex = (pingIndex + 1) % MAX_HISTORY_SIZE;

    if (pingCount < MAX_HISTORY_SIZE) {
        ++pingCount;
    }

    ImVec2 windowSizeImGui = ImVec2(windowSize.x * 0.1f, windowSize.y * 0.07f);
    ImVec2 plotSize = ImVec2(windowSizeImGui.x - 15, windowSizeImGui.y - 15);

    ImVec2 windowPos = ImVec2(
        windowSize.x - windowSizeImGui.x - ((windowSize.x + windowSize.y) * 0.002f) - (windowSize.x * 0.1f),
        (windowSize.x + windowSize.y) * 0.002f
    );
    ImGui::SetNextWindowPos(windowPos);
    ImGui::SetNextWindowSize(windowSizeImGui);

    ImGui::Begin(
        "Ping Monitor",
        nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_NoBackground
    );

    ImVec4 lineColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
    ImVec4 bgColor = ImVec4(0.1f, 0.1f, 0.1f, 0.5f);

    ImGui::PushStyleColor(ImGuiCol_PlotLines, lineColor);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, bgColor);

    ImGui::PlotLines(
        "", pingArray.data(), pingCount, pingIndex, std::format("Ping: {:.1f}ms", ping).c_str(), 0, 150, plotSize
    );

    ImGui::PopStyleColor(2);
    ImGui::End();
}
