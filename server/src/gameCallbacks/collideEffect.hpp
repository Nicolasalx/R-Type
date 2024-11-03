/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** collideEffect
*/

#pragma once

#include <list>
#include "Registry.hpp"
#include "TimeoutHandler.hpp"

namespace rts {

void collideEffect(
    ecs::Registry &reg,
    entity_t entityA,
    entity_t entityB,
    std::list<std::vector<char>> &datasToSend,
    ntw::UDPServer &udpServer,
    ntw::TimeoutHandler &timeoutHandler
);

} // namespace rts
