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

using callback_collide_function_t =
    void(ecs::Registry &reg, size_t entityA, size_t entityB, std::list<std::vector<char>> &datasToSend);

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
 * @param collideCallback Callback for collision effect between 2 entities
 */
void collision(
    Registry &reg,
    std::list<std::vector<char>> &datasToSend,
    const std::function<callback_collide_function_t> &collideCallback = {}
);
} // namespace ecs::systems
