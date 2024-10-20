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
    WindowMode windowMode = rtc::WindowMode::MENU;
    int fpsLimit = rt::CLIENT_FPS_LIMIT;
    bool chatEnable = false;

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
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::RControl) {
                chatEnable = !chatEnable;
            }
        }
        ImGui::SFML::Update(*window, dt.restart());
        window->clear();
        windowSize = window->getSize();

        if (windowMode == rtc::WindowMode::MENU) {
            menuWindow(*window, io, windowMode);
        } else if (windowMode == rtc::WindowMode::OPTIONS) {
            optionsWindow(*window, windowSize, fpsLimit, windowMode);
        } else {
            lobbyWindow(windowSize, roomManager);
        }
        renderChat(roomManager, windowSize, chatEnable);

        ImGui::SFML::Render(*window);
        window->display();
    }
}
