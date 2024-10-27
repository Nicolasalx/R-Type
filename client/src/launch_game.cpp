/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** launch_game
*/

#include <SFML/Window/VideoMode.hpp>
#include <cstddef>
#include <cstdio>
#include <memory>
#include <string>
#include "ClientEntityFactory.hpp"
#include "ClientTickRate.hpp"
#include "GameManager.hpp"
#include "InputManager.hpp"
#include "RTypeClient.hpp"
#include "RTypeConst.hpp"
#include "RTypeUDPProtol.hpp"
#include "Registry.hpp"
#include "RoomManager.hpp"
#include "SoundManager.hpp"
#include "SpriteManager.hpp"
#include "TickRateManager.hpp"
#include "TrackedException.hpp"
#include "imgui.h"
#include "udp/UDPClient.hpp"

static void spawnPlayer(ntw::UDPClient &udp, std::size_t userId, const rtc::RoomManager &roomManager)
{
    rt::UDPPacket<rt::UDPBody::NEW_ENTITY_PLAYER> msg(rt::UDPCommand::NEW_ENTITY_PLAYER, ecs::generateSharedEntityId());
    msg.body.playerIndex = 1;
    for (const auto &[id, player] : roomManager.getCurrentRoomPlayer()) {
        if (id == userId) {
            player.name.copy(msg.body.playerName, sizeof(msg.body.playerName) - 1);
            break;
        }
        ++msg.body.playerIndex;
    }
    msg.body.playerId = userId;
    msg.body.pos = {.x = 100, .y = 150 + (50 * float(msg.body.playerIndex))};
    udp.send(reinterpret_cast<const char *>(&msg), sizeof(msg));
}

void rtc::GameManager::_setupGui()
{
    if (!ImGui::SFML::Init(*_window, false)) {
        throw eng::TrackedException("IMGUI Window init failed");
    }
    ImGuiIO &io = ImGui::GetIO();
    io.Fonts->Clear();
    io.Fonts->AddFontDefault();

    ImFont *rawFont = io.Fonts->AddFontFromFileTTF("assets/font/DroidSansMono.ttf", 80.f);
    if (rawFont == nullptr) {
        throw eng::TrackedException("Failed to load font from assets/font/DroidSansMono.ttf");
    }
    _font = std::shared_ptr<ImFont>(rawFont, [](ImFont *) {});
    if (!ImGui::SFML::UpdateFontTexture()) {
        return;
    }
}

void rtc::GameManager::_setupEntities(ntw::UDPClient &udpClient, ecs::Registry &reg, ecs::SpriteManager &spriteManager)
{
    spawnPlayer(udpClient, _userId, *_roomManager);

    for (std::size_t i = 1; i <= 11; ++i) {
        ecs::ClientEntityFactory::createClientEntityFromJSON(
            reg, spriteManager, "assets/obstacles/obstacle" + std::to_string(i) + ".json"
        );
    }
    ecs::ClientEntityFactory::createClientEntityFromJSON(reg, spriteManager, "assets/bottomCollision.json");
    ecs::ClientEntityFactory::createClientEntityFromJSON(reg, spriteManager, "assets/topCollision.json");
    ecs::ClientEntityFactory::createClientEntityFromJSON(reg, spriteManager, "assets/rightCollision.json");
    ecs::ClientEntityFactory::createClientEntityFromJSON(reg, spriteManager, "assets/leftCollision.json");
    ecs::ClientEntityFactory::createClientEntityFromJSON(reg, spriteManager, "assets/bg.json");
    ecs::ClientEntityFactory::createClientEntityFromJSON(reg, spriteManager, "assets/earth.json", 500, 123);
    ecs::ClientEntityFactory::createClientEntityFromJSON(reg, spriteManager, "assets/planetShade75.json", 500, 123);
    ecs::ClientEntityFactory::createClientEntityFromJSON(reg, spriteManager, "assets/planet50.json", 1500, 303);
    ecs::ClientEntityFactory::createClientEntityFromJSON(reg, spriteManager, "assets/planetShade50.json", 1500, 303);
    ecs::ClientEntityFactory::createClientEntityFromJSON(reg, spriteManager, "assets/planetShade25.json", 1000, 288);
    ecs::ClientEntityFactory::createClientEntityFromJSON(reg, spriteManager, "assets/planetShade25.json", 1000, 288);
    ecs::ClientEntityFactory::createClientEntityFromJSON(reg, spriteManager, "assets/sun.json");
    ecs::ClientEntityFactory::createClientEntityFromJSON(reg, spriteManager, "assets/explosion.json", 300, 200);
}

void rtc::GameManager::_runGame()
{
    ntw::UDPClient udpClient(_ip, _gamePort);

    ecs::Registry reg;
    float dt = 0.f;
    ecs::InputManager inputManager;
    ntw::TickRateManager<rtc::TickRate> tickRateManager;
    ecs::SpriteManager spriteManager;
    ecs::SoundManager soundManager;

    _window->setView(_view);

    soundManager.loadMusic("battle", "assets/battle.ogg");
    soundManager.playMusic("battle", 5.f, true);
    soundManager.loadSoundBuffer("explosion", "assets/boom12.wav");
    soundManager.playSoundEffect("explosion", 100.f, false);

    rtc::registerComponents(reg);
    _networkCallbacks.registerConsumeFunc([&reg](auto f) { f(reg); });
    rtc::registerSystems(
        reg,
        *_window,
        dt,
        udpClient,
        inputManager,
        tickRateManager,
        spriteManager,
        _networkCallbacks,
        _metrics,
        _keyBind,
        soundManager
    );

    _setupUdpConnection(spriteManager, udpClient);

    std::future<bool> otherPlayer = _allUDPClientReady.get_future();
    _roomManager->udpConnectionReady();
    otherPlayer.wait();

    _setupEntities(udpClient, reg, spriteManager);
    runGameLoop(reg, _window, dt, inputManager, _gameState);
}

void rtc::GameManager::_launchGame()
{
    _window = std::make_shared<sf::RenderWindow>(sf::VideoMode(rt::SCREEN_WIDTH, rt::SCREEN_HEIGHT), "R-Type");
    _window->setFramerateLimit(rt::CLIENT_FPS_LIMIT);

    _setupGui();

    while (_gameState.load() != GameState::NONE) {
        switch (_gameState.load()) {
            case GameState::LOBBY:
                rtc::runGui(_window, _roomManager, _gameState, _keyBind);
                break;
            case GameState::GAME:
                _runGame();
                break;
            default:
                return;
        }
    }
    if (_window->isOpen()) {
        _window->close();
    }
}
