/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** draw_fps
*/

#pragma once

#include <SFML/Graphics.hpp>

namespace ecs::systems {

void drawFPS(float dt, const sf::Vector2u &windowSize);

} // namespace ecs::systems
