/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** run_gui
*/

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <cstring>
#include <memory>
#include <string>
#include "Logger.hpp"
#include "RTypeClient.hpp"
#include "RTypeConst.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "imgui-SFML.h"

void rtc::runGui(
    const std::shared_ptr<sf::RenderWindow> &window,
    const std::shared_ptr<rtc::RoomManager> &roomManager,
    bool &inLobby,
    ecs::KeyBind<rt::PlayerAction, sf::Keyboard::Key> &keyBind
)
{
    sf::Clock dt;
    sf::Vector2u windowSize;
    WindowMode windowMode = rtc::WindowMode::MENU;
    int fpsLimit = rt::CLIENT_FPS_LIMIT;
    bool chatEnable = false;
    bool scoreBoardEnable = false;
    sf::Texture texture;
    if (!texture.loadFromFile("assets/menu/background.jpg")) {
        eng::logError("Failed to load background image !");
    }
    sf::Sprite background(texture);
    background.setScale(rt::SCREEN_WIDTH / float(texture.getSize().x), rt::SCREEN_HEIGHT / float(texture.getSize().y));

    while (window->isOpen() && inLobby) {
        sf::Event event{};
        while (window->pollEvent(event)) {
            ImGui::SFML::ProcessEvent(*window, event);
            if (event.type == sf::Event::Closed) {
                window->close();
            } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::RControl) {
                chatEnable = !chatEnable;
            } else if (event.type == sf::Event::Resized) {
                windowSize = window->getSize();
            }
        }
        ImGui::SFML::Update(*window, dt.restart());
        window->clear();
        window->draw(background);
        switch (windowMode) {
            case rtc::WindowMode::EXIT_MENU:
                window->close();
                break;
            case rtc::WindowMode::MENU:
                menuWindow(windowSize, windowMode);
                break;
            case rtc::WindowMode::OPTIONS:
                optionsWindow(*window, windowSize, fpsLimit, windowMode);
                break;
            case rtc::WindowMode::ACCESSIBILITY:
                renderAccessibility(windowSize, windowMode, keyBind);
                break;
            case rtc::WindowMode::LOBBY:
                lobbyWindow(windowSize, roomManager, scoreBoardEnable);
                break;
        }
        renderChat(roomManager, windowSize, chatEnable);
        renderScoreBoard(windowSize, scoreBoardEnable);
        ImGui::SFML::Render(*window);
        window->display();
    }
}
