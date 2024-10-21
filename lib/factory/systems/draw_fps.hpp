/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** draw_fps
*/

#pragma once

#include <SFML/Graphics.hpp>
#include "MetricManager.hpp"

namespace ecs::systems {

void drawFPS(ecs::Metric &metric, float dt, const sf::Vector2u &windowSize);

} // namespace ecs::systems
