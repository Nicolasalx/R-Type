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
    float buttonWidth = windowSize.x * 0.20f;
    float buttonHeight = windowSize.y * 0.07f;

    ImGui::SetNextWindowPos(ImVec2(windowSize.x * 0.5f, windowSize.y * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(windowSize.x * 0.75f, windowSize.y * 0.75f));

    ImGui::Begin(
        "Options Window",
        nullptr,
        ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus
    );

    ImGui::SetWindowFontScale((windowSize.x + windowSize.y) / 1000.0f);

    ImGui::Text("Frame Rate Limit: %d", fpsLimit);
    ImGui::SliderInt("##fps_slider", &fpsLimit, 30, 144, "", ImGuiSliderFlags_AlwaysClamp);
    window.setFramerateLimit(fpsLimit);

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::Text("Resolution:");
    ImGui::Spacing();

    ImGui::BeginGroup();
    if (ImGui::Button("1920x1080", ImVec2(buttonWidth, buttonHeight))) {
        window.setSize(sf::Vector2u(1920, 1080));
    }
    ImGui::SameLine();
    if (ImGui::Button("1280x720", ImVec2(buttonWidth, buttonHeight))) {
        window.setSize(sf::Vector2u(1280, 720));
    }
    ImGui::SameLine();
    if (ImGui::Button("720x480", ImVec2(buttonWidth, buttonHeight))) {
        window.setSize(sf::Vector2u(720, 480));
    }
    ImGui::EndGroup();

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - buttonWidth) * 0.5f);
    if (ImGui::Button("Back", ImVec2(buttonWidth, buttonHeight))) {
        windowMode = rtc::WindowMode::MENU;
    }

    ImGui::End();
}
