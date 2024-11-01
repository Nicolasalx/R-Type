/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** gravity_system
*/

#include "systems/gravity_system.hpp"
#include "RTypeUDPProtol.hpp"
#include "Registry.hpp"
#include "Zipper.hpp"
#include "components/gravity.hpp"
#include "components/position.hpp"
#include "components/velocity.hpp"

void ecs::systems::gravitySystem(Registry &reg, const sf::Vector2u &windowSize)
{
    auto &gravities = reg.getComponents<ecs::component::Gravity>();
    auto &positions = reg.getComponents<ecs::component::Position>();
    auto &velocitys = reg.getComponents<ecs::component::Velocity>();

    ecs::Zipper<
        ecs::component::Gravity,
        ecs::component::Position,
        ecs::component::Velocity>
        zipParallax(gravities, positions, velocitys);

    for (auto [gravity, pos, vel] : zipParallax) {
        if (gravity.ennemyType == "ground") {
            vel.vy += 1.0f;
        }
    }
}
