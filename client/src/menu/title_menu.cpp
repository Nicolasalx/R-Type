/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** title_menu
*/

#include "../RTypeClient.hpp"
#include "RTypeConst.hpp"
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
    ImGui::SetWindowFontScale(1.5f);
    ImGui::Text("Limite de FPS : %d", fpsLimit);
    ImGui::SliderInt("##fps_slider", &fpsLimit, 30, 144, "", ImGuiSliderFlags_AlwaysClamp);
    window.setFramerateLimit(fpsLimit);
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Text("Redimensionner la fenêtre :");
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
    if (ImGui::Button("Retour", ImVec2(300, 50))) {
        windowMode = rtc::WindowMode::MENU;
    }
    ImGui::End();
}

void rtc::menuWindow(sf::RenderWindow &window, ImGuiIO &io, WindowMode &windowMode)
{
    ImGui::SetNextWindowSize(ImVec2(window.getSize().x, window.getSize().y));
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin(
        "Menu Titre",
        nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus
    );
    float windowWidth = ImGui::GetWindowSize().x;
    float windowHeight = ImGui::GetWindowSize().y;
    ImGui::Dummy(ImVec2(0.0f, windowHeight * 0.3f));
    ImGui::PushFont(io.Fonts->Fonts[0]);
    float buttonWidth = 200.0f;
    float buttonHeight = 60.0f;
    ImGui::SetCursorPosX((windowWidth - buttonWidth) / 2);
    if (ImGui::Button("PLAY", ImVec2(buttonWidth, buttonHeight))) {
        windowMode = rtc::WindowMode::LOBBY;
    }
    ImGui::Dummy(ImVec2(0.0f, 20.0f));
    ImGui::SetCursorPosX((windowWidth - buttonWidth) / 2);
    if (ImGui::Button("OPTIONS", ImVec2(buttonWidth, buttonHeight))) {
        windowMode = rtc::WindowMode::OPTIONS;
    }
    ImGui::Dummy(ImVec2(0.0f, 20.0f));
    ImGui::SetCursorPosX((windowWidth - buttonWidth) / 2);
    if (ImGui::Button("QUIT", ImVec2(buttonWidth, buttonHeight))) {
        window.close();
    }
    ImGui::PopFont();
    ImGui::End();
}

static void renderInsideRoom(rtc::RoomManager &roomManager, const sf::Vector2u &windowSize)
{
    // ! Window
    ImGui::SetNextWindowSize(ImVec2(windowSize.x, windowSize.y));
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin(
        roomManager.getCurrentRoom().c_str(),
        nullptr,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus
    );

    // ! Table
    ImGui::BeginTable("playersTable", 2);
    ImGui::TableSetupColumn("Name");
    ImGui::TableSetupColumn("Status");
    ImGui::TableHeadersRow();
    ImU32 selfRowColor = ImGui::GetColorU32(ImVec4(0.5, 1, 0.5, 0.25));

    // ! Action with table
    for (const auto &[id, player] : roomManager.getCurrentRoomPlayer()) {
        ImGui::TableNextRow();
        if (id == roomManager.getSelfId()) {
            ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, selfRowColor);
        }
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("%s", player.name.c_str());
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%s", player.ready ? "ready" : "not ready");
    }
    ImGui::EndTable();

    // ! Back to lobby button
    {
        ImVec2 buttonSize(200, 50);
        ImVec2 windowContentRegionMax = ImGui::GetWindowContentRegionMax();
        ImVec2 buttonPos =
            ImVec2(windowContentRegionMax.x - buttonSize.x - 20, windowContentRegionMax.y - buttonSize.y - 20);
        ImGui::SetCursorPos(buttonPos);
        if (ImGui::Button("Back to lobby", buttonSize)) {
            // ! send leave
            roomManager.askToLeaveRoom();
        }
    }

    // ! ready button
    ImVec2 buttonSize(200, 50);
    ImVec2 windowContentRegionMax = ImGui::GetWindowContentRegionMax();
    ImVec2 buttonPos = ImVec2(20, windowContentRegionMax.y - buttonSize.y - 20);
    ImGui::SetCursorPos(buttonPos);
    if (!roomManager.getCurrentRoomPlayer().at(roomManager.getSelfId()).ready) {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.8f, 0.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.6f, 0.0f, 1.0f));
        if (ImGui::Button("Ready", buttonSize)) {
            roomManager.askToBeReady();
        }
    } else {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.0f, 0.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.6f, 0.0f, 0.0f, 1.0f));
        if (ImGui::Button("Not Ready", buttonSize)) {
            roomManager.askToBeNotReady();
        }
    }
    ImGui::PopStyleColor(3);

    ImGui::End();
}

static void unsupportedWindowSize(const sf::Vector2u &windowSize)
{
    ImGui::SetNextWindowSize(ImVec2(windowSize.x, windowSize.y));
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin(
        "Window to small",
        nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove
    );
    ImGui::Text("Window size not supported !");
    ImGui::End();
}

void rtc::lobbyWindow(sf::Vector2u &windowSize, rtc::RoomManager &roomManager)
{
    if (windowSize.x < rt::MIN_SCREEN_WIDTH || windowSize.y < rt::MIN_SCREEN_HEIGHT) {
        unsupportedWindowSize(windowSize);
    } else if (roomManager.getCurrentRoom().empty()) {
        rtc::renderLobbyWindow(roomManager, windowSize);
    } else {
        renderInsideRoom(roomManager, windowSize);
    }
}
