/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** draw_fps
*/

#include "systems/draw_fps.hpp"
#include <format>
#include "RTypeConst.hpp"
#include "imgui.h"

void ecs::systems::drawFPS(ecs::Metric &metric, float dt, const sf::Vector2u &windowSize)
{
    metric.lastComputedMetric = 1.0f / dt;
    metric.addNewValue(metric.lastComputedMetric);

    ImVec2 windowSizeImGui = ImVec2(windowSize.x * 0.1f, windowSize.y * 0.07f);
    ImVec2 plotSize = ImVec2(windowSizeImGui.x - 15, windowSizeImGui.y - 15);

    ImVec2 windowPos = ImVec2(
        windowSize.x - windowSizeImGui.x - ((windowSize.x + windowSize.y) * 0.002f),
        (windowSize.x + windowSize.y) * 0.002f
    );
    ImGui::SetNextWindowPos(windowPos);
    ImGui::SetNextWindowSize(windowSizeImGui);

    ImGui::Begin(
        "FPS Monitor",
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
        "",
        metric.getArray().data(),
        metric.getCount(),
        metric.getIndex(),
        std::format("FPS: {:.1f}", metric.lastComputedMetric).c_str(),
        0,
        rt::CLIENT_FPS_LIMIT * 2,
        plotSize
    );

    ImGui::PopStyleColor(2);
    ImGui::End();
}
