/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** health_shared_check
*/

#pragma once

#include <list>
#include "Registry.hpp"

/**
 * @brief Perform health checks on entities within the registry.
 *
 * This function iterates through the entities in the provided registry and performs necessary health checks
 * on entities marked as shared client and server sides.
 *
 * @param reg Reference to the registry containing the entities to be checked.
 * @param datasToSend Reference of a list of data that will be sent to all the clients.
 */
namespace ecs::systems {

void healthSharedCheck(Registry &reg, std::list<std::vector<char>> &datasToSend);

} // namespace ecs::systems
