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
    float &currentYOffset // Pass in the current Y offset to stack buttons
)
{
    float buttonWidth = windowSize.x * 0.25f;   // Button width
    float buttonHeight = windowSize.y * 0.08f;  // Button height
    float buttonSpacing = windowSize.y * 0.02f; // Spacing between buttons

    // Set button color
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.8f, 0.7f));        // Semi-transparent blue background
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.4f, 1.0f, 0.9f)); // Lighten on hover
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.2f, 0.6f, 1.0f));  // Darker when clicked

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);               // Rounded corners
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.0f, 10.0f)); // Padding for better text alignment

    // Set button position at the bottom left corner
    ImGui::SetCursorPosX(windowSize.x * 0.05f); // 5% from the left edge
    ImGui::SetCursorPosY(currentYOffset);       // Use currentYOffset for Y position

    // Render the button and handle the action
    if (ImGui::Button(name.c_str(), ImVec2(buttonWidth, buttonHeight))) {
        windowMode = newWindowMode;
    }

    // Increase the current Y offset for the next button
    currentYOffset += buttonHeight + buttonSpacing;

    ImGui::PopStyleColor(3); // Restore original button color
    ImGui::PopStyleVar(2);   // Restore frame padding and rounding
}

void rtc::menuWindow(const sf::Vector2u &windowSize, WindowMode &windowMode)
{
    // Set up window size and position
    ImGui::SetNextWindowSize(ImVec2(windowSize.x, windowSize.y));
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin(
        "Title Menu",
        nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoBringToFrontOnFocus
    );

    // Set font scaling based on window size for responsiveness
    ImGui::SetWindowFontScale((windowSize.x + windowSize.y) / 1200.0);

    // Apply transparent background for buttons area
    ImGui::Dummy(ImVec2(0.0f, windowSize.y * 0.75f)); // Reserve 75% for background image

    // Initialize Y offset for buttons
    float currentYOffset = windowSize.y * 0.575f; // Start from 75% of window height

    // Render buttons, each time increasing the Y offset
    renderMenuButton("PLAY", rtc::WindowMode::LOBBY, windowSize, windowMode, currentYOffset);
    renderMenuButton("OPTIONS", rtc::WindowMode::OPTIONS, windowSize, windowMode, currentYOffset);
    renderMenuButton("ACCESSIBILITY", rtc::WindowMode::ACCESSIBILITY, windowSize, windowMode, currentYOffset);
    renderMenuButton("QUIT", rtc::WindowMode::EXIT_MENU, windowSize, windowMode, currentYOffset);

    ImGui::End();
}
