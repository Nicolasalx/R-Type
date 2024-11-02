/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** check_out_of_range
*/

#include "systems/check_out_of_range.hpp"
#include "IndexedZipper.hpp"
#include "RTypeUDPProtol.hpp"
#include "components/health.hpp"
#include "components/shared_entity.hpp"

void ecs::systems::checkOutOfRange(
    ecs::Registry &reg,
    WaveManager &waveManager,
    std::list<std::vector<char>> &datasToSend
)
{
    auto &healths = reg.getComponents<ecs::component::Health>();
    auto &sharedIds = reg.getComponents<ecs::component::SharedEntity>();

    ecs::IndexedZipper<component::Health, ecs::component::SharedEntity> zipHealth(healths, sharedIds);

    for (auto [entityId, health, sharedId] : zipHealth) {
        if (waveManager.isMob(entityId) && waveManager.isOutOfBonds(reg, entityId)) {
            waveManager.removeEntity(entityId);
            datasToSend.push_back(
                rt::UDPPacket<rt::UDPBody::DEL_ENTITY>(rt::UDPCommand::DEL_ENTITY, sharedId.sharedEntityId, true)
                    .serialize()
            );
            reg.killEntity(entityId);
        }
    }
}
