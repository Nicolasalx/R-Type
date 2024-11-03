/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** waveAi
*/

#include <cmath>
#include <cstdio>
#include <list>

#include "Registry.hpp"
#include "TimeoutHandler.hpp"
#include "WaveManager.hpp"

namespace rts::ais {

void splitAi(
    ecs::Registry &reg,
    entity_t e,
    std::list<std::vector<char>> &datasToSend,
    ntw::UDPServer &udpServer,
    ntw::TimeoutHandler &timeoutHandler,
    ecs::WaveManager &waveManager
);

} // namespace rts::ais
