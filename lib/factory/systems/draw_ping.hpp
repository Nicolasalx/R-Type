/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** draw_fps
*/

#pragma once

#include <SFML/Graphics.hpp>
#include "Registry.hpp"

namespace ecs::systems {

void drawPing(Registry &reg, const sf::Vector2u &windowSize);

} // namespace ecs::systems
