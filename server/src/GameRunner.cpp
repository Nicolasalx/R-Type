/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** game_runner
*/

#include "GameRunner.hpp"
#include <cstddef>
#include <string>
#include "FrameRateManager.hpp"
#include "IndexedZipper.hpp"
#include "Logger.hpp"
#include "RTypeConst.hpp"
#include "RTypeServer.hpp"
#include "RTypeUDPProtol.hpp"
#include "Registry.hpp"
#include "asio/ip/udp.hpp"
#include "components/player.hpp"
#include "components/shared_entity.hpp"

rts::GameRunner::GameRunner(int port, std::size_t stage, bool debugMode) // ! Use the stage argument
    : _udpServer(port), _debugMode(debugMode)
{
    eng::logWarning("Selected stage: " + std::to_string(stage) + ".");

    _networkCallbacks.registerConsumeFunc([this](auto f) { f(_reg); });
    rts::registerUdpResponse(_responseHandler, _datasToSend, _networkCallbacks, _udpServer);
    _udpServer.registerCommand([this](udp::endpoint &endpoint, char *data, std::size_t size) {
        this->_responseHandler.handleResponse(data, size, {std::ref(endpoint)});
    });
    _udpServer.run();

    rts::registerComponents(_reg);
    rts::registerSystems(
        _reg, _window, _dt, _tickRateManager, _udpServer, _datasToSend, _networkCallbacks, _waveManager, debugMode
    );
    rts::initWaves(_waveManager, _datasToSend);
}

void rts::GameRunner::killPlayer(size_t playerId)
{
    _networkCallbacks.pushBack([playerId, this](ecs::Registry &reg) {
        ecs::IndexedZipper<ecs::component::Player, ecs::component::SharedEntity> zip(
            reg.getComponents<ecs::component::Player>(), reg.getComponents<ecs::component::SharedEntity>()
        );

        for (auto [e, player, shared] : zip) {
            if (player.id == playerId) {
                _datasToSend.push_back(
                    rt::UDPPacket<rt::UDPBody::DEL_ENTITY>(rt::UDPCommand::DEL_ENTITY, shared.sharedEntityId)
                        .serialize()
                );
                reg.killEntity(e);
                return;
            }
        }
    });
}

void rts::GameRunner::addWindow(const sf::VideoMode &videomode, const std::string &title)
{
    if (!_debugMode) {
        return;
    }
    _window.create(videomode, title);
    _window.setFramerateLimit(rt::SERVER_ENGINE_TARGET_FPS);
}

void rts::GameRunner::_runGameDebug(bool &stopGame)
{
    sf::Clock clock;

    while (_window.isOpen() && !stopGame) {
        _dt = clock.restart().asSeconds();

        sf::Event event{};
        while (_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                _window.close();
            }
        }
        _window.clear();
        _reg.runSystems();
        _window.display();
    }
    if (_window.isOpen()) {
        _window.close();
    }
}

void rts::GameRunner::runGame(bool &stopGame)
{
    eng::FramerateManager frameRate(rt::SERVER_ENGINE_TARGET_FPS);
    sf::Clock clock;

    if (_debugMode) {
        _runGameDebug(stopGame);
        return;
    }
    while (!stopGame) {
        frameRate.start();
        _dt = clock.restart().asSeconds();
        _reg.runSystems();
        frameRate.end();
    }
}
