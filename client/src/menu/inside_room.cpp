/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** inside_room
*/

#include "../RTypeClient.hpp"
#include "imgui.h"

void rtc::renderInsideRoom(rtc::RoomManager &roomManager, const sf::Vector2u &windowSize)
{
    ImGui::SetNextWindowSize(ImVec2(windowSize.x, windowSize.y));
    ImGui::SetNextWindowPos(ImVec2(0, 0));

    ImGui::Begin(
        roomManager.getCurrentRoom().c_str(),
        nullptr,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus
    );

    ImGui::SetWindowFontScale((windowSize.x + windowSize.y) / 1000.0f);

    ImGui::BeginTable("playersTable", 2);
    ImGui::TableSetupColumn("Name");
    ImGui::TableSetupColumn("Status");
    ImGui::TableHeadersRow();

    ImU32 selfRowColor = ImGui::GetColorU32(ImVec4(0.5f, 1.0f, 0.5f, 0.25f));

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

    ImVec2 buttonSize(windowSize.x * 0.25f, windowSize.y * 0.15f);
    float padding = windowSize.x * 0.02f;

    ImVec2 windowContentRegionMax = ImGui::GetWindowContentRegionMax();
    ImVec2 leaveButtonPos = ImVec2(windowContentRegionMax.x - buttonSize.x - padding, windowContentRegionMax.y - buttonSize.y - padding);
    ImGui::SetCursorPos(leaveButtonPos);
    if (ImGui::Button("Leave Room", buttonSize)) {
        roomManager.askToLeaveRoom();
    }

    ImVec2 readyButtonPos = ImVec2(padding, windowContentRegionMax.y - buttonSize.y - padding);
    ImGui::SetCursorPos(readyButtonPos);

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
