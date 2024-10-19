/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** draw_fps
*/

#pragma once

#include <SFML/Graphics.hpp>
#include "../../../client/src/Metric.hpp"

namespace ecs::systems {

void drawPing(rtc::Metric &metric, const sf::Vector2u &windowSize);

} // namespace ecs::systems
