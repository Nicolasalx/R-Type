/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** collision
*/

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cstddef>
#include <list>

#include "Registry.hpp"
#include "components/controllable.hpp"
#include "components/gravity.hpp"
#include "components/hitbox.hpp"
#include "components/missile.hpp"
#include "components/player.hpp"
#include "components/position.hpp"
#include "components/velocity.hpp"
#include "systems/collision.hpp"

static void resolveCollision(
    ecs::component::Position &pos,
    const sf::FloatRect &intersection,
    std::optional<ecs::component::Velocity> &vel
)
{
    if (!vel) {
        return;
    }

    if (intersection.width < intersection.height) {
        if (vel->vx > 0) {
            pos.x -= intersection.width;
        } else if (vel->vx < 0) {
            pos.x += intersection.width;
        }
        vel->vx = 0;
    } else {
        if (vel->vy > 0) {
            pos.y -= intersection.height;
        } else if (vel->vy < 0) {
            pos.y += intersection.height;
        }
        vel->vy = 0;
    }
}

// NOLINTBEGIN(bugprone-unchecked-optional-access)

namespace ecs::systems {

void collision(
    Registry &reg,
    std::list<std::vector<char>> &datasToSend,
    const std::function<callback_collide_function_t> &collideCallback
)
{
    auto &positions = reg.getComponents<ecs::component::Position>();
    auto &hitboxes = reg.getComponents<ecs::component::Hitbox>();
    auto &velocities = reg.getComponents<ecs::component::Velocity>();
    auto &controllables = reg.getComponents<ecs::component::Controllable>();
    auto &gravities = reg.getComponents<ecs::component::Gravity>();
    auto &players = reg.getComponents<ecs::component::Player>();

    size_t maxEntity = std::max(positions.size(), hitboxes.size());

    for (size_t entityA = 0; entityA < maxEntity; ++entityA) {
        if (!positions.has(entityA) || !hitboxes.has(entityA)) {
            continue;
        }
        auto &posA = *positions[entityA];
        auto &hbA = *hitboxes[entityA];
        sf::FloatRect rectA(posA.x, posA.y, hbA.width, hbA.height);

        for (size_t entityB = entityA + 1; entityB < maxEntity; ++entityB) {
            if (!positions.has(entityB) || !hitboxes.has(entityB)) {
                continue;
            }
            auto &posB = *positions[entityB];
            auto &hbB = *hitboxes[entityB];
            sf::FloatRect rectB(posB.x, posB.y, hbB.width, hbB.height);

            sf::FloatRect intersection;
            if (rectA.intersects(rectB, intersection)) {
                bool entityAControllable = controllables.has(entityA);
                bool entityBControllable = controllables.has(entityB);
                bool entityAGravity = gravities.has(entityA);
                bool entityBGravity = gravities.has(entityB);
                const bool entityAPlayer = players.has(entityA);
                const bool entityBPlayer = players.has(entityB);

                if (entityAControllable && !entityBControllable) {
                    if (!entityAPlayer) {
                        resolveCollision(posA, intersection, velocities[entityA]);
                    }
                } else if (!entityAControllable && entityBControllable) {
                    if (!entityBPlayer) {
                        resolveCollision(posB, intersection, velocities[entityB]);
                    }
                } else if (entityAGravity && !entityBGravity) {
                    resolveCollision(posA, intersection, velocities[entityA]);
                } else if (!entityAGravity && entityBGravity) {
                    resolveCollision(posB, intersection, velocities[entityB]);
                }
                // TODO: If both entities are controllable or both are non-controllable
                collideCallback(reg, entityA, entityB, datasToSend);
            }
        }
    }
}

void collisionPredict(Registry &reg)
{
    auto &positions = reg.getComponents<ecs::component::Position>();
    auto &hitboxes = reg.getComponents<ecs::component::Hitbox>();
    auto &velocities = reg.getComponents<ecs::component::Velocity>();
    auto &controllables = reg.getComponents<ecs::component::Controllable>();
    auto &gravities = reg.getComponents<ecs::component::Gravity>();
    auto &players = reg.getComponents<ecs::component::Player>();
    auto &missiles = reg.getComponents<ecs::component::Missile>();

    size_t maxEntity = std::max(positions.size(), hitboxes.size());

    for (size_t entityA = 0; entityA < maxEntity; ++entityA) {
        if (!positions.has(entityA) || !hitboxes.has(entityA)) {
            continue;
        }
        auto &posA = *positions[entityA];
        auto &hbA = *hitboxes[entityA];
        sf::FloatRect rectA(posA.x, posA.y, hbA.width, hbA.height);

        for (size_t entityB = entityA + 1; entityB < maxEntity; ++entityB) {
            if (!positions.has(entityB) || !hitboxes.has(entityB)) {
                continue;
            }
            auto &posB = *positions[entityB];
            auto &hbB = *hitboxes[entityB];
            sf::FloatRect rectB(posB.x, posB.y, hbB.width, hbB.height);

            sf::FloatRect intersection;
            if (rectA.intersects(rectB, intersection)) {
                bool entityAControllable = controllables.has(entityA);
                bool entityBControllable = controllables.has(entityB);
                bool entityAGravity = gravities.has(entityA);
                bool entityBGravity = gravities.has(entityB);
                const bool entityAPlayer = players.has(entityA);
                const bool entityBPlayer = players.has(entityB);
                const bool entityAMissile = missiles.has(entityA);
                const bool entityBMissile = missiles.has(entityB);

                if (entityAControllable && !entityBControllable) {
                    if (entityAPlayer && entityBMissile) {
                        continue;
                    }
                    resolveCollision(posA, intersection, velocities[entityA]);
                } else if (!entityAControllable && entityBControllable) {
                    if (entityBPlayer && entityAMissile) {
                        continue;
                    }
                    resolveCollision(posB, intersection, velocities[entityB]);
                } else if (entityAGravity && !entityBGravity) {
                    resolveCollision(posA, intersection, velocities[entityA]);
                } else if (!entityAGravity && entityBGravity) {
                    resolveCollision(posB, intersection, velocities[entityB]);
                }
                // TODO: If both entities are controllable or both are non-controllable
            }
        }
    }
}

// NOLINTEND(bugprone-unchecked-optional-access)

} // namespace ecs::systems
