/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** title_menu
*/

#include "../RTypeClient.hpp"
#include "imgui.h"

static void renderMenuButton(
    const std::string &name,
    rtc::WindowMode newWindowMode,
    const sf::Vector2u &windowSize,
    rtc::WindowMode &windowMode,
    float &currentYOffset
)
{
    float buttonWidth = windowSize.x * 0.25f;
    float buttonHeight = windowSize.y * 0.08f;
    float buttonSpacing = windowSize.y * 0.02f;

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.8f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.4f, 1.0f, 0.9f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.2f, 0.6f, 1.0f));

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.0f, 10.0f));

    ImGui::SetCursorPosX(windowSize.x * 0.05f);
    ImGui::SetCursorPosY(currentYOffset);

    if (ImGui::Button(name.c_str(), ImVec2(buttonWidth, buttonHeight))) {
        windowMode = newWindowMode;
    }

    currentYOffset += buttonHeight + buttonSpacing;

    ImGui::PopStyleColor(3);
    ImGui::PopStyleVar(2);
}

void rtc::menuWindow(const sf::Vector2u &windowSize, WindowMode &windowMode)
{
    ImGui::SetNextWindowSize(ImVec2(windowSize.x, windowSize.y));
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin(
        "Title Menu",
        nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoBringToFrontOnFocus
    );

    ImGui::SetWindowFontScale((windowSize.x + windowSize.y) / 1200.0);

    ImGui::Dummy(ImVec2(0.0f, windowSize.y * 0.75f));

    float currentYOffset = windowSize.y * 0.575f;

    renderMenuButton("PLAY", rtc::WindowMode::LOBBY, windowSize, windowMode, currentYOffset);
    renderMenuButton("OPTIONS", rtc::WindowMode::OPTIONS, windowSize, windowMode, currentYOffset);
    renderMenuButton("ACCESSIBILITY", rtc::WindowMode::ACCESSIBILITY, windowSize, windowMode, currentYOffset);
    renderMenuButton("QUIT", rtc::WindowMode::EXIT_MENU, windowSize, windowMode, currentYOffset);

    ImGui::End();
}
