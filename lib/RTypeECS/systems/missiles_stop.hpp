/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** missiles_stop
*/

#pragma once

#include <list>
#include "Registry.hpp"
#include "TimeoutHandler.hpp"

/**
 * @brief Stops the missiles in the ECS registry.
 *
 * This function is responsible for stopping the missiles that are currently
 * active in the ECS registry. It processes the entities and components
 * related to missiles and kill the missiles entities when needed.
 *
 * @param reg Reference to the ECS registry containing the entities and components.
 * @param datasToSend Reference of a list of data that will be sent to all the clients.
 */
namespace ecs::systems {

void missilesStop(
    ecs::Registry &reg,
    std::list<std::vector<char>> &datasToSend,
    ntw::UDPServer &udpServer,
    ntw::TimeoutHandler &timeoutHandler
);

} // namespace ecs::systems
