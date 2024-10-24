/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** game_manager
*/

#pragma once

#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <functional>
#include <memory>
#include "KeyBind.hpp"
#include "MetricManager.hpp"
#include "RTypeConst.hpp"
#include "Registry.hpp"
#include "RoomManager.hpp"
#include "SafeList.hpp"
#include "SpriteManager.hpp"
#include "TCPResponseHandler.hpp"
#include "UDPResponseHandler.hpp"
#include "imgui.h"
#include "tcp/TCPClient.hpp"
#include "udp/UDPClient.hpp"
#include "shared_entity.hpp"
#include <imgui-SFML.h>
#include <unordered_map>

namespace rtc {

class GameManager {
    private:
    std::string _ip;
    std::string _playerName;

    std::shared_ptr<ntw::TCPClient> _tcpClient;
    bool _inLobby = true;
    std::size_t _userId = 0;
    int _gamePort = 0;
    std::shared_ptr<rtc::RoomManager> _roomManager;
    rt::TCPResponseHandler _tcpResponseHandler;
    rt::UDPResponseHandler _udpResponseHandler;
    std::promise<bool> _allUDPClientReady;

    eng::SafeList<std::function<void(ecs::Registry &reg)>> _networkCallbacks;
    std::shared_ptr<sf::RenderWindow> _window;
    sf::View _view = {
        sf::Vector2f(rt::GAME_VIEW_WIDTH / 2.0, rt::GAME_VIEW_HEIGHT / 2.0),
        sf::Vector2f(rt::GAME_VIEW_WIDTH, rt::GAME_VIEW_HEIGHT)
    };
    std::shared_ptr<ImFont> _font;

    ecs::MetricManager<rt::GameMetric> _metrics = {{{rt::GameMetric::FPS, 100}, {rt::GameMetric::PING, 100}}};

    ecs::KeyBind<rt::PlayerAction, sf::Keyboard::Key> _keyBind{
        {{rt::PlayerAction::MOVE_UP, sf::Keyboard::Key::Up},
         {rt::PlayerAction::MOVE_DOWN, sf::Keyboard::Key::Down},
         {rt::PlayerAction::MOVE_LEFT, sf::Keyboard::Key::Left},
         {rt::PlayerAction::MOVE_RIGHT, sf::Keyboard::Key::Right},
         {rt::PlayerAction::SHOOT_MISSILE, sf::Keyboard::Key::Space}}
    };

    void _registerTcpResponse();
    void _registerUdpResponse(ecs::SpriteManager &spriteManager, ntw::UDPClient &udpClient);

    void _setupTcpConnection();
    void _setupUdpConnection(ecs::SpriteManager &spriteManager, ntw::UDPClient &udpClient);
    void _launchGame();

    public:
    GameManager(const std::string &ip, int port, const std::string &playerName)
        : _ip(ip), _playerName(playerName), _userId(ecs::generateSharedEntityId())
    {
        _tcpClient = std::make_shared<ntw::TCPClient>(ip, port);
        _roomManager = std::make_shared<rtc::RoomManager>(_tcpClient, _userId, playerName);
    }

    ~GameManager()
    {
        ImGui::SFML::Shutdown();
    }

    void runGame()
    {
        _setupTcpConnection();
        _launchGame();
    }
};
} // namespace rtc
