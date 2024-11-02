/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** draw_radial_light
*/

#pragma once

#include "Registry.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

/**
 * @namespace ecs::systems
 * @brief Contains system functions for managing game logic.
 */

/**
 * @brief Renders radial light effects on the screen.
 *
 * The `renderRadialLight` function processes and displays radial
 * light effects in the game, enhancing the visual atmosphere.
 * It uses the current game state and the specified render window
 * to draw the light effects appropriately.
 *
 * @param reg Reference to the game registry that holds all game entities and components.
 * @param window Reference to the SFML render window where the radial light effects will be drawn.
 */
namespace ecs::systems {
void renderRadialLight(Registry &reg, sf::RenderWindow &window);
} // namespace ecs::systems
