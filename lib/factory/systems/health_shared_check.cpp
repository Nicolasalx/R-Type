/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** health_shared_check
*/

#include "health_shared_check.hpp"
#include "IndexedZipper.hpp"
#include "RTypeUDPProtol.hpp"
#include "components/health.hpp"
#include "components/shared_entity.hpp"

void ecs::systems::healthSharedCheck(Registry &reg, std::list<std::vector<char>> &datasToSend)
{
    auto &healths = reg.getComponents<ecs::component::Health>();
    auto &shared = reg.getComponents<ecs::component::SharedEntity>();

    IndexedZipper<component::Health, ecs::component::SharedEntity> zip(healths, shared);

    for (auto [entityId, health, sharedId] : zip) {
        if (health.currHp <= 0) {
            datasToSend.push_back(
                rt::UDPPacket<rt::UDPBody::DEL_ENTITY>(rt::UDPCommand::DEL_ENTITY, sharedId.sharedEntityId).serialize()
            );
            reg.killEntity(entityId);
        }
    }
}
