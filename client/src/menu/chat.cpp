/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** chat
*/

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "../RTypeClient.hpp"
#include "RTypeConst.hpp"
#include "imgui.h"

void rtc::renderChat(rtc::RoomManager &roomManager, const sf::Vector2u &windowSize, bool chatEnable)
{
    float chatWidth = windowSize.x * 0.30f;
    float chatHeight = windowSize.y * 0.25f;
    float chatPosX = windowSize.x - chatWidth - 10.0f;
    float chatPosY = windowSize.y - chatHeight - 10.0f;

    if (!chatEnable) {
        return;
    }

    ImGui::SetNextWindowPos(ImVec2(chatPosX, chatPosY));
    ImGui::SetNextWindowSize(ImVec2(chatWidth, chatHeight));

    ImGui::Begin("Chat", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::BeginChild(
        "ChatMessages", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), true, ImGuiWindowFlags_HorizontalScrollbar
    );

    for (const auto &msg : roomManager.getChatMsg()) {

        ImGui::TextWrapped("%s", msg.c_str());
    }

    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
        ImGui::SetScrollHereY(1);
    }

    ImGui::EndChild();

    static char buff[rt::MAX_CHAT_MSG_SIZE + 1] = {0};
    if (ImGui::InputText("Send", buff, rt::MAX_CHAT_MSG_SIZE, ImGuiInputTextFlags_EnterReturnsTrue)) {
        if (buff[0]) {
            roomManager.askToSendChatMsg(roomManager.getSelfName() + ": " + std::string(buff));
            buff[0] = '\0';
        }
        ImGui::SetKeyboardFocusHere(-1);
    }

    ImGui::End();
}
