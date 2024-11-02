/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** particle_spawner
*/

#pragma once

#include "Registry.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

namespace ecs::systems {

void drawParticle(ecs::Registry &reg, const sf::Time &dt, sf::RenderWindow &window);

}
