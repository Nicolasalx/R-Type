/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** draw_player_beam_bar
*/

#pragma once

#include <SFML/Graphics.hpp>
#include "Registry.hpp"

/**
 * @namespace ecs::systems
 * @brief Contains system functions for managing game logic.
 */

/**
 * @brief Renders the player's score on the screen.
 *
 * The `drawScore` function displays the current score of the player
 * in the game. It updates the visual representation of the score based
 * on the player's current score and the specified window size.
 *
 * @param reg Reference to the game registry that holds all game entities and components.
 * @param windowSize The size of the game window, used for positioning the score display.
 * @param score Reference to the integer representing the player's score.
 */
namespace ecs::systems {
void drawScore(Registry &reg, const sf::Vector2u &windowSize, int &score);
} // namespace ecs::systems
