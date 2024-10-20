/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** game_manager
*/

#pragma once

#include <functional>
#include <list>
#include <memory>
#include "Metric.hpp"
#include "RTypeClientConst.hpp"
#include "Registry.hpp"
#include "RoomManager.hpp"
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

    ntw::TCPClient _tcpClient;
    bool _inLobby = true;
    std::size_t _userId = 0;
    int _gamePort = 0;
    rtc::RoomManager _roomManager;
    rt::TCPResponseHandler _tcpResponseHandler;
    rt::UDPResponseHandler _udpResponseHandler;
    std::promise<bool> _allUDPClientReady;

    std::list<std::function<void(ecs::Registry &reg)>> _networkCallbacks;
    std::shared_ptr<sf::RenderWindow> _window;
    std::shared_ptr<ImFont> _font;

    std::unordered_map<rtc::ClientMetric, rtc::Metric> _metrics;

    void _registerTcpResponse();
    void _registerUdpResponse(ecs::SpriteManager &spriteManager);

    void _setupTcpConnection();
    void _setupUdpConnection(ecs::SpriteManager &spriteManager, ntw::UDPClient &udpClient);
    void _launchGame();

    public:
    GameManager(const std::string &ip, int port, const std::string &playerName)
        : _ip(ip), _playerName(playerName), _tcpClient(ip, port), _userId(ecs::generateSharedEntityId()),
          _roomManager(_tcpClient, _userId, playerName),
          _metrics(
              {{rtc::ClientMetric::FPS, Metric(rtc::METRIC_HISTORY_SIZE.at(rtc::ClientMetric::FPS))},
               {rtc::ClientMetric::PING, Metric(rtc::METRIC_HISTORY_SIZE.at(rtc::ClientMetric::PING))}}
          )
    {
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
