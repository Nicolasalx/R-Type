/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** option
*/

#include "../RTypeClient.hpp"
#include "imgui.h"
#include "imgui-SFML.h"

void rtc::optionsWindow(sf::RenderWindow &window, sf::Vector2u windowSize, int &fpsLimit, WindowMode &windowMode)
{
    ImGui::SetNextWindowPos(ImVec2(windowSize.x / 2.0f, windowSize.y / 2.0f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(500, 400));
    ImGui::Begin(
        "InvisibleWindow",
        nullptr,
        ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus
    );
    ImGui::SetWindowFontScale(2);
    ImGui::Text("Frame Rate Limit: %d", fpsLimit);
    ImGui::SliderInt("##fps_slider", &fpsLimit, 30, 144, "", ImGuiSliderFlags_AlwaysClamp);
    window.setFramerateLimit(fpsLimit);
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Text("Resolution:");
    ImGui::Spacing();
    ImGui::BeginGroup();
    if (ImGui::Button("1920x1080", ImVec2(150, 50))) {
        window.setSize(sf::Vector2u(1920, 1080));
    }
    ImGui::SameLine();
    if (ImGui::Button("1280x720", ImVec2(150, 50))) {
        window.setSize(sf::Vector2u(1280, 720));
    }
    ImGui::SameLine();
    if (ImGui::Button("640x480", ImVec2(150, 50))) {
        window.setSize(sf::Vector2u(640, 480));
    }
    ImGui::EndGroup();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - 300) * 0.5f);
    if (ImGui::Button("Back", ImVec2(300, 50))) {
        windowMode = rtc::WindowMode::MENU;
    }
    ImGui::End();
}
