/*
** EPITECH PROJECT, 2024
** Rtype
** File description:
** server_share_movement
*/

#pragma once

#include <list>
#include "Registry.hpp"

namespace ecs::systems {

/**
 * @brief Updates the position of all entities based on their velocity.
 *
 * The `position` function iterates through all entities with both `position` and
 * `velocity` components, updating their positions based on their velocities and
 * the elapsed time `dt`.
 *
 * @param reg Reference to the registry managing entities and their components.
 * @param datasToSend Reference of a list of data that will be sent to all the clients.
 */
void serverShareMovement(Registry &reg, std::list<std::vector<char>> &datasToSend);

} // namespace ecs::systems
