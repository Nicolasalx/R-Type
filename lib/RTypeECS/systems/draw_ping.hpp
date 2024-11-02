/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** draw_fps
*/

#pragma once

#include <SFML/Graphics.hpp>
#include "MetricManager.hpp"

/**
 * @namespace ecs::systems
 * @brief Contains system functions for managing game logic.
 */

/**
 * @brief Renders the network ping information on the screen.
 *
 * The `drawPing` function displays the current network ping status
 * on the game window. This helps in monitoring the game's network
 * performance during gameplay.
 *
 * @param metric Reference to the `Metric` manager that holds the ping data.
 * @param windowSize The size of the game window, used for positioning the ping display.
 */
namespace ecs::systems {
void drawPing(ecs::Metric &metric, const sf::Vector2u &windowSize);
} // namespace ecs::systems
