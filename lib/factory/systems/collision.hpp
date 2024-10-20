/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** collision
*/

#pragma once

#include <list>
#include <vector>
#include "Registry.hpp"

namespace ecs::systems {

/**
 * @brief Handles collision detection and response for entities.
 *
 * The `collision` function iterates through entities managed by the provided
 * `registry`, checks for collisions between entities with `hitbox` and `position` components,
 * and updates their states accordingly. This function is part of the systems that
 * process game logic in the ECS (Entity-Component-System) architecture.
 *
 * @param reg Reference to the registry managing entities and their components.
 */
void collisionPredict(Registry &reg);

/**
 * @brief Handles collision detection and response for entities.
 *
 * The `collision` function iterates through entities managed by the provided
 * `registry`, checks for collisions between entities with `hitbox` and `position` components,
 * and updates their states accordingly. This function is part of the systems that
 * process game logic in the ECS (Entity-Component-System) architecture.
 *
 * @param reg Reference to the registry managing entities and their components.
 * @param datasToSend Reference of a list of data that will be sent to all the clients.
 */
void collision(Registry &reg, std::list<std::vector<char>> &datasToSend);
} // namespace ecs::systems
