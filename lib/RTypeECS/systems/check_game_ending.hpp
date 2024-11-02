/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** check_game_ending
*/

#pragma once

#include <functional>
#include "Registry.hpp"

/**
 * @namespace ecs::systems
 * @brief Contains system functions for managing game logic.
 */

/**
 * @brief Checks the conditions for ending the game.
 *
 * The `checkGameEnding` function evaluates whether the game should
 * end based on a given predicate function. If the conditions are met,
 * it executes a callback function to handle the end of the game logic.
 *
 * @param reg Reference to the game registry that holds all game entities and components.
 * @param pred A predicate function that takes a reference to the registry and returns
 *             a boolean indicating whether the game should end.
 * @param callback A callback function that will be executed if the game-ending
 *                 conditions are satisfied. It takes a reference to the registry.
 */
namespace ecs::systems {
void checkGameEnding(
    ecs::Registry &reg,
    const std::function<bool(ecs::Registry &)> &pred,
    const std::function<void(ecs::Registry &)> &callback
);
} // namespace ecs::systems
