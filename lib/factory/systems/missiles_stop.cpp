/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** missiles_stop
*/

#include "missiles_stop.hpp"
#include <cstdio>

#include "IndexedZipper.hpp"
#include "RTypeConst.hpp"
#include "RTypeUDPProtol.hpp"
#include "components/missile.hpp"
#include "components/position.hpp"
#include "components/shared_entity.hpp"

// NOLINTBEGIN(bugprone-unchecked-optional-access)

void ecs::systems::missilesStop(ecs::Registry &reg, std::list<std::vector<char>> &datasToSend)
{
    auto &missiles = reg.getComponents<ecs::component::Missile>();
    auto &positions = reg.getComponents<ecs::component::Position>();

    ecs::IndexedZipper<ecs::component::Position, ecs::component::Missile> zip(positions, missiles);

    for (auto [entityId, pos, missile] : zip) {
        if (pos.x < 0 || pos.y < 0 || pos.x > rt::SCREEN_WIDTH || pos.y > rt::SCREEN_HEIGHT) {
            if (reg.hasComponent<component::SharedEntity>(entityId)) {
                auto sharedId = reg.getComponent<ecs::component::SharedEntity>(entityId)->sharedEntityId;
                datasToSend.push_back(
                    rt::UDPPacket<rt::UDPBody::DEL_ENTITY>(rt::UDPCommand::DEL_ENTITY, sharedId).serialize()
                );
            }
            reg.killEntity(entityId);
        }
    }
}

// NOLINTEND(bugprone-unchecked-optional-access)
