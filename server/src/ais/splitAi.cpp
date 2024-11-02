/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** splitAi
*/

#include "splitAi.hpp"
#include "RTypeUDPProtol.hpp"
#include "ServerEntityFactory.hpp"
#include "components/health.hpp"
#include "components/position.hpp"
#include "fireRandomMissileAi.hpp"
#include "waveAi.hpp"
#include "components/ai_actor.hpp"
#include "components/death_split.hpp"

void rts::ais::splitAi(
    ecs::Registry &reg,
    entity_t e,
    std::list<std::vector<char>> &datasToSend,
    ecs::WaveManager &waveManager
)
{
    if (!reg.hasComponent<ecs::component::Position>(e) || !reg.hasComponent<ecs::component::DeathSplit>(e) ||
        !reg.hasComponent<ecs::component::Health>(e) || !reg.hasComponent<ecs::component::SharedEntity>(e)) {
        return;
    }

    auto &pos = reg.getComponent<ecs::component::Position>(e).value();
    auto &health = reg.getComponent<ecs::component::Health>(e).value();
    auto &split = reg.getComponent<ecs::component::DeathSplit>(e).value();
    auto &sharedEntity = reg.getComponent<ecs::component::SharedEntity>(e).value();
    if (split.splitCount <= 0) {
        reg.removeComponent<ecs::component::DeathSplit>(e);
        return;
    }

    if (health.currHp <= 1 && split.splitCount > 0) {
        for (const auto &offset : split.offsets) {
            float newX = pos.x + offset.first;
            float newY = pos.y + offset.second;
            shared_entity_t newSharedId = ecs::generateSharedEntityId();

            datasToSend.push_back(
                rt::UDPPacket<rt::UDPBody::NEW_ENTITY_BLOB>(
                    rt::UDPCommand::NEW_ENTITY_BLOB, newSharedId, {.pos = {.x = newX, .y = newY}}, true
                )
                    .serialize()
            );

            auto newEntity = ecs::ServerEntityFactory::createServerEntityFromJSON(
                reg, "assets/enemies/bydosWave.json", newX, newY, newSharedId, -10.0f
            );

            if (split.splitCount >= 1) {
                reg.addComponent(
                    newEntity, ecs::component::DeathSplit{.splitCount = split.splitCount - 1, .offsets = split.offsets}
                );
                reg.getComponent<ecs::component::AiActor>(e)->act =
                    [newY, &datasToSend, &waveManager](ecs::Registry &r, entity_t ent) {
                        rts::ais::waveMovement(r, ent, newY);
                        rts::ais::fireRandomMissileAi(r, ent, datasToSend, 250);
                        rts::ais::splitAi(r, ent, datasToSend, waveManager);
                    };
            } else {
                reg.getComponent<ecs::component::AiActor>(e)->act =
                    [newY, &datasToSend, &waveManager](ecs::Registry &r, entity_t ent) {
                        rts::ais::waveMovement(r, ent, newY);
                        rts::ais::fireRandomMissileAi(r, ent, datasToSend, 250);
                        rts::ais::splitAi(r, ent, datasToSend, waveManager);
                    };
            }
        }

        datasToSend.push_back(
            rt::UDPPacket<rt::UDPBody::DEL_ENTITY>(rt::UDPCommand::DEL_ENTITY, sharedEntity.sharedEntityId, true)
                .serialize()
        );
        waveManager.removeEntity(e);
        reg.killEntity(e);
    }
}
