/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** draw_radial_ligh
*/

#pragma once

#include "Registry.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

namespace ecs::systems {

void renderRadialLight(Registry &reg, sf::RenderWindow &window);

} // namespace ecs::systems
