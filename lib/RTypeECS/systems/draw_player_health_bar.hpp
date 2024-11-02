/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** draw_player_health_bar
*/

#pragma once

#include <SFML/Graphics.hpp>
#include "Registry.hpp"

/**
 * @namespace ecs::systems
 * @brief Contains system functions for managing game logic.
 */

/**
 * @brief Renders the player's health bar on the screen.
 *
 * The `drawPlayerHealthBar` function displays the current health
 * status of the player character. It updates the visual representation
 * of the health bar based on the player's current health and the
 * specified window size.
 *
 * @param reg Reference to the game registry that holds all game entities and components.
 * @param windowSize The size of the game window, used for positioning the health bar.
 */
namespace ecs::systems {
void drawPlayerHealthBar(Registry &reg, const sf::Vector2u &windowSize);
} // namespace ecs::systems
