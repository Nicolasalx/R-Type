/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** gravity_system
*/

#include "systems/gravity_system.hpp"
#include "components/gravity.hpp"
#include "components/position.hpp"
#include "Zipper.hpp"

#include <iostream>

void ecs::systems::gravitySystem(Registry &reg, const sf::Vector2u &windowSize)
{
    auto &gravities = reg.getComponents<ecs::component::Gravity>();
    auto &positions = reg.getComponents<ecs::component::Position>();

    ecs::Zipper<ecs::component::Gravity, ecs::component::Position> zipParallax(gravities, positions);

    int index = 0;
    for (const auto &[gravity, position] : zipParallax) {
        if (gravity.ennemyType == "ground") {
            position.y += 20;
            std::cout << "ROBOT N°" << index << " => POSITION X: " << position.x << " / POSITION Y: " << position.y << "\n";
            ++index;
        }
    }
}
