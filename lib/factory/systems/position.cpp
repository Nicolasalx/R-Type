/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** position
*/

#include "components/position.hpp"
#include "Registry.hpp"
#include "Zipper.hpp"
#include "components/gravity.hpp"
#include "components/velocity.hpp"

namespace ecs::systems {

void position(Registry &reg, float dt)
{
    auto &gravities = reg.getComponents<ecs::component::Gravity>();
    auto &positions = reg.getComponents<ecs::component::Position>();
    auto &velocities = reg.getComponents<ecs::component::Velocity>();

    ecs::Zipper<ecs::component::Gravity, ecs::component::Position, ecs::component::Velocity> zipGravity(
        gravities, positions, velocities
    );

    for (auto [gravity, pos, vel] : zipGravity) {
        if (gravity.ennemyType == "ground") {
            vel.vy += 1.0f;
        }
    }

    ecs::Zipper<ecs::component::Position, ecs::component::Velocity> zip(positions, velocities);

    for (auto [pos, vel] : zip) {
        pos.x += vel.vx * dt;
        pos.y += vel.vy * dt;
    }
}

} // namespace ecs::systems
