/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** collideEffect
*/

#include "collideEffect.hpp"
#include <cstdio>

#include "RTypeUDPProtol.hpp"
#include "components/health.hpp"
#include "components/missile.hpp"
#include "components/tag.hpp"
#include "entity.hpp"
#include "components/game_tag.hpp"

// NOLINTBEGIN(bugprone-unchecked-optional-access)

static void tagEffectKill(
    ecs::Registry &reg,
    entity_t entity,
    std::list<std::vector<char>> &datasToSend,
    ntw::UDPServer &udpServer,
    ntw::TimeoutHandler &timeoutHandler
)
{
    if (reg.hasComponent<ecs::component::SharedEntity>(entity)) {
        auto sharedId = reg.getComponent<ecs::component::SharedEntity>(entity)->sharedEntityId;
        auto newMsg = rt::UDPPacket<rt::UDPBody::DEL_ENTITY>(rt::UDPCommand::DEL_ENTITY, sharedId, true);
        timeoutHandler.addTimeoutPacket(newMsg.serialize(), newMsg.packetId, udpServer);
        datasToSend.push_back(std::move(newMsg).serialize());
    }
    reg.killEntity(entity);
}

static void tagEffectDamage(
    ecs::Registry &reg,
    entity_t entity,
    int damage,
    std::list<std::vector<char>> &datasToSend,
    ntw::UDPServer &udpServer,
    ntw::TimeoutHandler &timeoutHandler
)
{
    if (!reg.hasComponent<ecs::component::Health>(entity)) {
        tagEffectKill(reg, entity, datasToSend, udpServer, timeoutHandler);
        return;
    }
    auto &health = reg.getComponents<ecs::component::Health>();

    health[entity]->currHp -= damage;
    if (reg.hasComponent<ecs::component::SharedEntity>(entity)) {
        auto sharedId = reg.getComponent<ecs::component::SharedEntity>(entity)->sharedEntityId;
        datasToSend.push_back(rt::UDPPacket<rt::UDPBody::TAKE_DAMAGE>(
                                  rt::UDPCommand::TAKE_DAMAGE, sharedId, rt::UDPBody::TAKE_DAMAGE{.damage = damage}
        )
                                  .serialize());
    }
}

static void resolveDamage(
    ecs::Registry &reg,
    entity_t ally,
    entity_t enemy,
    std::list<std::vector<char>> &datasToSend,
    ntw::UDPServer &udpServer,
    ntw::TimeoutHandler &timeoutHandler
)
{
    auto &missiles = reg.getComponents<ecs::component::Missile>();

    bool isAllyMissile = missiles.has(ally);
    bool isEnemyMissile = missiles.has(enemy);

    if (isAllyMissile && isEnemyMissile) {
        return;
    }
    if (isAllyMissile && !isEnemyMissile) {
        tagEffectDamage(reg, enemy, missiles[ally]->damage, datasToSend, udpServer, timeoutHandler);
        tagEffectKill(reg, ally, datasToSend, udpServer, timeoutHandler);
        return;
    }
    if (isEnemyMissile && !isAllyMissile) {
        tagEffectDamage(reg, ally, missiles[enemy]->damage, datasToSend, udpServer, timeoutHandler);
        tagEffectKill(reg, enemy, datasToSend, udpServer, timeoutHandler);
        return;
    }
    tagEffectDamage(reg, ally, 1, datasToSend, udpServer, timeoutHandler);
}

void rts::collideEffect(
    ecs::Registry &reg,
    entity_t entityA,
    entity_t entityB,
    std::list<std::vector<char>> &datasToSend,
    ntw::UDPServer &udpServer,
    ntw::TimeoutHandler &timeoutHandler
)
{
    auto &tags = reg.getComponents<ecs::component::Tag<ecs::component::EntityTag>>();

    if (!tags.has(entityA) || !tags.has(entityB)) { // check again if it can happen
        return;
    }

    auto [tagA] = *tags[entityA];
    auto [tagB] = *tags[entityB];

    if (tagA == ecs::component::EntityTag::NONE || tagB == ecs::component::EntityTag::NONE) {
        return;
    }
    if (tagA == ecs::component::EntityTag::ALLY && tagB == ecs::component::EntityTag::ENEMY) {
        resolveDamage(reg, entityA, entityB, datasToSend, udpServer, timeoutHandler);
    }
    if (tagB == ecs::component::EntityTag::ALLY && tagA == ecs::component::EntityTag::ENEMY) {
        resolveDamage(reg, entityB, entityA, datasToSend, udpServer, timeoutHandler);
    }
}

// NOLINTEND(bugprone-unchecked-optional-access)
