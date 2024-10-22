/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** radial_light
*/

#pragma once

#include "Candle/RadialLight.hpp"
#include "SFML/System/Vector2.hpp"

namespace ecs::component {

struct RadialLight {
    sf::Vector2f offset;
    candle::RadialLight light;
};

} // namespace ecs::component
