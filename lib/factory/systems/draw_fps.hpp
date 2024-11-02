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
 * @brief Draws the frames per second (FPS) on the screen.
 *
 * The `drawFPS` function renders the current frames per second (FPS)
 * information onto the game window. It updates the display based on the
 * metric information and the elapsed time since the last frame.
 *
 * @param metric Reference to the `Metric` manager that holds FPS data.
 * @param dt The elapsed time since the last update, typically measured in seconds.
 * @param windowSize The size of the game window, used for positioning the FPS display.
 */
namespace ecs::systems {
void drawFPS(ecs::Metric &metric, float dt, const sf::Vector2u &windowSize);
} // namespace ecs::systems
