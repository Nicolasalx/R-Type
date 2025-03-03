/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** RTypeServer
*/

#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <atomic>
#include <list>
#include <sys/types.h>
#include <vector>
#include "Registry.hpp"
#include "RoomManager.hpp"
#include "ServerTickRate.hpp"
#include "TCPResponseHandler.hpp"
#include "TickRateManager.hpp"
#include "UDPResponseHandler.hpp"
#include "WaveManager.hpp"
#include "udp/UDPServer.hpp"

namespace rts {
int parseArg(int argc, const char **argv, rts::RoomManager &roomManager, int &port);
void registerComponents(ecs::Registry &reg);
void registerSystems(
    ecs::Registry &reg,
    sf::RenderWindow &window,
    float &dt,
    ntw::TickRateManager<rts::TickRate> &tickRateManager,
    ntw::UDPServer &udpServer,
    std::list<std::vector<char>> &datasToSend,
    ntw::TimeoutHandler &timeoutHandler,
    eng::SafeList<std::function<void(ecs::Registry &reg)>> &networkCallbacks,
    ecs::WaveManager &waveManager,
    bool debugMode,
    size_t &nbPlayers
);
void registerUdpResponse(
    rt::UDPResponseHandler &responseHandler,
    std::list<std::vector<char>> &datasToSend,
    eng::SafeList<std::function<void(ecs::Registry &reg)>> &networkCallbacks,
    ntw::UDPServer &udpServer,
    ntw::TimeoutHandler &timeoutHandler,
    std::atomic<bool> &stopGame
);
void registerTcpResponse(
    rts::RoomManager &roomManager,
    ntw::TCPServer &tcpServer,
    rt::TCPResponseHandler &responseHandler
);

void initWaves(ecs::WaveManager &waveManager, std::list<std::vector<char>> &datasToSend, int missileSpawnRate);
void commandHandler(std::string &str, rts::RoomManager &roomManager, ntw::TCPServer &tcpServer);
} // namespace rts
