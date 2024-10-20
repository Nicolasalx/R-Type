/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** accessibility
*/

#include "../KeyboardMap.hpp"
#include "../RTypeClient.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "imgui.h"

static void displayKeyBind(
    const std::string &name,
    rt::PlayerAction action,
    ecs::KeyBind<rt::PlayerAction, sf::Keyboard::Key> &keyBind,
    float buttonWidth,
    float buttonHeight
)
{
    static bool waitingForKey = false;
    static rt::PlayerAction currentAction = rt::PlayerAction::MOVE_UP;

    ImGui::Text("%s: %s", name.c_str(), rtc::KEYBOARD_MAP.at(keyBind.getActionKey(action)).c_str());
    ImGui::SameLine();

    if (waitingForKey && currentAction == action) {
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() - buttonWidth - ImGui::GetStyle().WindowPadding.x);
        if (ImGui::Button(("Press any key##" + name).c_str(), ImVec2(buttonWidth, buttonHeight))) {
            currentAction = action;
            waitingForKey = true;
        }
        for (int key = 0; key < sf::Keyboard::KeyCount; ++key) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(key))) {
                keyBind.changeActionKey(currentAction, sf::Keyboard::Key(key));
                waitingForKey = false;
                break;
            }
        }
    } else {
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() - buttonWidth - ImGui::GetStyle().WindowPadding.x);
        if (ImGui::Button(("Change##" + name).c_str(), ImVec2(buttonWidth, buttonHeight))) {
            currentAction = action;
            waitingForKey = true;
        }
    }
}

void rtc::renderAccessibility(
    sf::Vector2u windowSize,
    WindowMode &windowMode,
    ecs::KeyBind<rt::PlayerAction, sf::Keyboard::Key> &keyBind
)
{
    float buttonWidth = windowSize.x * 0.20f;
    float buttonHeight = windowSize.y * 0.05f;
    float paddingY = windowSize.y * 0.02f;

    ImGui::SetNextWindowPos(ImVec2(windowSize.x * 0.5f, windowSize.y * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(windowSize.x * 0.75f, windowSize.y * 0.75f));

    ImGui::Begin(
        "Accessibility Menu",
        nullptr,
        ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus
    );

    ImGui::SetWindowFontScale((windowSize.x + windowSize.y) / 1000.0f);

    ImGui::Text("Key Binding");
    ImGui::Dummy(ImVec2(0.0f, paddingY));

    displayKeyBind("Move Up", rt::PlayerAction::MOVE_UP, keyBind, buttonWidth, buttonHeight);
    displayKeyBind("Move Down", rt::PlayerAction::MOVE_DOWN, keyBind, buttonWidth, buttonHeight);
    displayKeyBind("Move Left", rt::PlayerAction::MOVE_LEFT, keyBind, buttonWidth, buttonHeight);
    displayKeyBind("Move Right", rt::PlayerAction::MOVE_RIGHT, keyBind, buttonWidth, buttonHeight);
    displayKeyBind("Shoot Missile", rt::PlayerAction::SHOOT_MISSILE, keyBind, buttonWidth, buttonHeight);

    ImGui::Dummy(ImVec2(0.0f, paddingY));
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0.0f, paddingY * 1.5f));

    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - buttonWidth) * 0.5f);
    if (ImGui::Button("Back", ImVec2(buttonWidth, buttonHeight * 1.2f))) {
        windowMode = rtc::WindowMode::MENU;
    }

    ImGui::End();
}
