/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** health_local_check
*/

#include "health_local_check.hpp"
#include "IndexedZipper.hpp"
#include "components/health.hpp"
#include "components/shared_entity.hpp"

void ecs::systems::healthLocalCheck(Registry &reg)
{
    auto &healths = reg.getComponents<ecs::component::Health>();

    IndexedZipper<component::Health> zip(healths);

    for (auto [entityId, health] : zip) {
        if (health.currHp <= 0 && !reg.hasComponent<component::SharedEntity>(entityId)) {
            reg.killEntity(entityId);
        }
    }
}
