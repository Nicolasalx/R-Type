/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** RTypeClient
*/

#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <sys/types.h>
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
#include "imgui.h"
#include "udp/UDPClient.hpp"

namespace rtc {

enum class WindowMode : std::uint8_t {
    EXIT_MENU,
    MENU,
    OPTIONS,
    ACCESSIBILITY,
    LOBBY
};

enum class GameState : std::uint8_t {
    NONE,
    LOBBY,
    GAME,

    WIN,
    LOSE
};

void registerComponents(ecs::Registry &reg);
void registerGameSystems(
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
    ecs::SoundManager &soundManager,
    int &score,
    sf::Clock &chargeClock
);

void registerEndingSystems(
    ecs::Registry &reg,
    sf::RenderWindow &window,
    const std::shared_ptr<ImFont> &font,
    std::atomic<GameState> &gameState,
    const std::string &playerName,
    const int &score
);

void runGameLoop(
    ecs::Registry &reg,
    const std::shared_ptr<sf::RenderWindow> &window,
    float &dt,
    ecs::InputManager &input,
    std::atomic<GameState> &gameState
);
void runGui(
    const std::shared_ptr<sf::RenderWindow> &window,
    const std::shared_ptr<rtc::RoomManager> &roomManager,
    std::atomic<GameState> &gameState,
    ecs::KeyBind<rt::PlayerAction, sf::Keyboard::Key> &keyBind
);
void renderInsideRoom(const std::shared_ptr<rtc::RoomManager> &roomManager, const sf::Vector2u &windowSize);
void renderLobbyWindow(
    const std::shared_ptr<rtc::RoomManager> &roomManager,
    const sf::Vector2u &windowSize,
    bool &scoreBoardEnable
);
void optionsWindow(sf::RenderWindow &window, sf::Vector2u windowSize, int &fpsLimit, WindowMode &windowMode);
void menuWindow(const sf::Vector2u &windowSize, WindowMode &windowMode);
void lobbyWindow(
    sf::Vector2u &windowSize,
    const std::shared_ptr<rtc::RoomManager> &roomManager,
    bool &scoreBoardEnable
);
void renderChat(const std::shared_ptr<rtc::RoomManager> &roomManager, const sf::Vector2u &windowSize, bool chatEnable);
void renderAccessibility(
    sf::Vector2u windowSize,
    WindowMode &windowMode,
    ecs::KeyBind<rt::PlayerAction, sf::Keyboard::Key> &keyBind
);
void renderScoreBoard(const sf::Vector2u &windowSize, bool scoreBoardEnable, const sf::Vector2u &pos);
void addScore(const std::string &filename, const std::string &playerName, int score);

} // namespace rtc
