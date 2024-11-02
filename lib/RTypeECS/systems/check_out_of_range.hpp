/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** check_out_of_range
*/

#pragma once

#include "Registry.hpp"
#include "WaveManager.hpp"

#include <list>

/**
 * @namespace ecs::systems
 * @brief Contains system functions for managing game logic.
 */

/**
 * @brief Checks for entities that are out of range.
 *
 * The `checkOutOfRange` function evaluates the positions of entities in the
 * game and determines if any are out of the defined range. If so, it
 * can update the wave manager and prepare data for sending.
 *
 * @param reg Reference to the game registry that holds all game entities and components.
 * @param waveManager Reference to the WaveManager that handles wave-related logic.
 * @param datasToSend A list of vectors of characters used to prepare data to send
 *                    based on the entities' status.
 */
namespace ecs::systems {
void checkOutOfRange(ecs::Registry &reg, WaveManager &waveManager, std::list<std::vector<char>> &datasToSend);
} // namespace ecs::systems
