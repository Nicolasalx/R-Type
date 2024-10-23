/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** RTypeClient
*/

#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include "ClientTickRate.hpp"
#include "InputManager.hpp"
#include "KeyBind.hpp"
#include "MetricManager.hpp"
#include "RTypeConst.hpp"
#include "Registry.hpp"
#include "RoomManager.hpp"
#include "SafeList.hpp"
#include "SoundManager.hpp"
#include "SpriteManager.hpp"
#include "TickRateManager.hpp"
#include "udp/UDPClient.hpp"

namespace rtc {

enum class WindowMode : std::uint8_t {
    EXIT_MENU,
    MENU,
    OPTIONS,
    ACCESSIBILITY,
    LOBBY
};

void registerComponents(ecs::Registry &reg);
void registerSystems(
    ecs::Registry &reg,
    sf::RenderWindow &window,
    float &dt,
    ntw::UDPClient &udpClient,
    ecs::InputManager &input,
    ntw::TickRateManager<rtc::TickRate> &tickRateManager,
    ecs::SpriteManager &spriteManager,
    eng::SafeList<std::function<void(ecs::Registry &reg)>> &networkCallbacks,
    ecs::MetricManager<rt::GameMetric> &metrics,
    const ecs::KeyBind<rt::PlayerAction, sf::Keyboard::Key> &keyBind,
    ecs::SoundManager &soundManager
);

void run(ecs::Registry &reg, const std::shared_ptr<sf::RenderWindow> &window, float &dt, ecs::InputManager &input);
void runGui(
    const std::shared_ptr<sf::RenderWindow> &window,
    rtc::RoomManager &roomManager,
    bool &inLobby,
    ecs::KeyBind<rt::PlayerAction, sf::Keyboard::Key> &keyBind
);
void renderInsideRoom(rtc::RoomManager &roomManager, const sf::Vector2u &windowSize);
void renderLobbyWindow(rtc::RoomManager &roomManager, const sf::Vector2u &windowSize);
void optionsWindow(sf::RenderWindow &window, sf::Vector2u windowSize, int &fpsLimit, WindowMode &windowMode);
void menuWindow(const sf::Vector2u &windowSize, WindowMode &windowMode);
void lobbyWindow(sf::Vector2u &windowSize, rtc::RoomManager &roomManager);
void renderChat(rtc::RoomManager &roomManager, const sf::Vector2u &windowSize, bool chatEnable);
void renderAccessibility(
    sf::Vector2u windowSize,
    WindowMode &windowMode,
    ecs::KeyBind<rt::PlayerAction, sf::Keyboard::Key> &keyBind
);

} // namespace rtc
