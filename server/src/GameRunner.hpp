/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** game_runner
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <cstddef>
#include <functional>
#include <list>
#include <vector>
#include "Registry.hpp"
#include "SafeList.hpp"
#include "ServerTickRate.hpp"
#include "TickRateManager.hpp"
#include "TimeoutHandler.hpp"
#include "UDPResponseHandler.hpp"
#include "WaveManager.hpp"
#include "udp/UDPServer.hpp"
#include "waveCreator/WaveCreator.hpp"

namespace rts {

class GameRunner {
    private:
    ntw::UDPServer _udpServer;
    ntw::TimeoutHandler _timeoutHandler;
    rt::UDPResponseHandler _responseHandler;

    size_t _nbPlayers;
    ecs::Registry _reg;
    ecs::WaveManager _waveManager;
    rts::WaveCreator _waveCreator;

    float _dt = 0;
    ntw::TickRateManager<rts::TickRate> _tickRateManager;

    std::list<std::vector<char>> _datasToSend;
    eng::SafeList<std::function<void(ecs::Registry &reg)>> _networkCallbacks;

    sf::RenderWindow _window;

    bool _debugMode = false;
    std::atomic<bool> _stopGame;

    void _runGameDebug();

    public:
    GameRunner(int port, std::size_t stage, int missileSpawnRate, bool debugMode, size_t nbPlayers);

    void setGameState(bool state)
    {
        _stopGame.store(state);
    }

    void killPlayer(size_t playerId);
    void addWindow(const sf::VideoMode &videomode, const std::string &title);
    void runGame();
};
} // namespace rts
