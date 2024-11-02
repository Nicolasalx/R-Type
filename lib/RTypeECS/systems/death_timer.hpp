/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** death_timer
*/

#pragma once

#include "Registry.hpp"

/**
 * @namespace ecs::systems
 * @brief Contains system functions for managing game logic.
 */

/**
 * @brief Updates the death timer for entities in the game.
 *
 * The `deathTimer` function processes the death timers of entities within
 * the game registry, decrementing their timers based on the elapsed time
 * since the last update. This function is crucial for managing timed
 * destruction of entities.
 *
 * @param reg Reference to the game registry that holds all game entities and components.
 * @param dt The elapsed time since the last update, typically measured in seconds.
 */
namespace ecs::systems {
void deathTimer(Registry &reg, float dt);
} // namespace ecs::systems
