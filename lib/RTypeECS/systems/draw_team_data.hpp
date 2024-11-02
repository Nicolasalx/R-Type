/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** draw_team_data
*/

#pragma once

#include <SFML/Graphics.hpp>
#include "Registry.hpp"

/**
 * @namespace ecs::systems
 * @brief Contains system functions for managing game logic.
 */

/**
 * @brief Renders the team data on the screen.
 *
 * The `drawTeamData` function displays information related to the
 * players' teams in the game. This may include details such as
 * team scores, members, or other relevant statistics, updated
 * according to the current state of the game.
 *
 * @param reg Reference to the game registry that holds all game entities and components.
 * @param windowSize The size of the game window, used for positioning the team data display.
 */
namespace ecs::systems {
void drawTeamData(Registry &reg, const sf::Vector2u &windowSize);
} // namespace ecs::systems
