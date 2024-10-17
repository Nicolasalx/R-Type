/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** run_gui
*/

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <cstring>
#include "RTypeClient.hpp"
#include "RTypeConst.hpp"
#include "imgui.h"
#include "imgui-SFML.h"

void rtc::runGui(const std::shared_ptr<sf::RenderWindow> &window, rtc::RoomManager &roomManager, bool &inLobby)
{
    sf::Clock dt;
    sf::Vector2u windowSize;
    WindowMode windowMode = MENU;
    int fpsLimit = rt::CLIENT_FPS_LIMIT;

    ImGuiIO &io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("assets/font/DroidSansMono.ttf", 32.0f);
    if (!ImGui::SFML::UpdateFontTexture()) {
        return;
    };

    while (window->isOpen() && inLobby) {
        sf::Event event{};
        while (window->pollEvent(event)) {
            ImGui::SFML::ProcessEvent(*window, event);
            if (event.type == sf::Event::Closed) {
                window->close();
            }
        }
        ImGui::SFML::Update(*window, dt.restart());
        window->clear();
        windowSize = window->getSize();

        if (windowMode == MENU) {
            menuWindow(*window, io, windowMode);
        } else if (windowMode == OPTIONS) {
            optionsWindow(*window, windowSize, fpsLimit, windowMode);
        } else {
            lobbyWindow(windowSize, roomManager);
        }

        ImGui::SFML::Render(*window);
        window->display();
    }
}
