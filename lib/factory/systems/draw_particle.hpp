/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** particle_spawner
*/

#pragma once

#include "Registry.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

/**
 * @namespace ecs::systems
 * @brief Contains system functions for managing game logic.
 */

/**
 * @brief Renders particles to the screen.
 *
 * The `drawParticle` function processes and draws all particle entities
 * to the specified SFML render window. It updates the particles based
 * on the elapsed time since the last frame.
 *
 * @param reg Reference to the game registry that holds all game entities and components.
 * @param dt The elapsed time since the last update, typically measured in seconds.
 * @param window Reference to the SFML render window where particles will be drawn.
 */
namespace ecs::systems {
void drawParticle(ecs::Registry &reg, const sf::Time &dt, sf::RenderWindow &window);
} // namespace ecs::systems
