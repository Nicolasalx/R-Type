#pragma once

#include "death_timer.hpp"
#include "components/death_timer.hpp"

namespace ecs::systems {

void deathTimer(Registry &reg, float dt)
{
    auto &deathTimers = reg.getComponents<ecs::component::DeathTimer>();

    for (size_t entityId = 0; entityId < deathTimers.size(); entityId++) {
        if (deathTimers[entityId]) {
            deathTimers[entityId]->timeToDeath -= dt;
            if (deathTimers[entityId]->timeToDeath <= 0) {
                reg.killEntity(entityId);
            }
        }
    }
}

} // namespace ecs::systems
