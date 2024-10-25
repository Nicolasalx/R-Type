/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** collideEffect
*/

#include "collideEffect.hpp"
#include "RTypeUDPProtol.hpp"

#include "components/health.hpp"
#include "components/missile.hpp"

// NOLINTBEGIN(bugprone-unchecked-optional-access)

static void tagEffectKill(ecs::Registry &reg, entity_t entity, std::list<std::vector<char>> &datasToSend)
{
    if (reg.hasComponent<ecs::component::SharedEntity>(entity)) {
        auto sharedId = reg.getComponent<ecs::component::SharedEntity>(entity)->sharedEntityId;
        datasToSend.push_back(
            rt::UDPPacket<rt::UDPBody::DEL_ENTITY>(rt::UDPCommand::DEL_ENTITY, sharedId, true).serialize()
        );
    }
    reg.killEntity(entity);
}

static void tagEffectDamage(ecs::Registry &reg, entity_t entity, int damage, std::list<std::vector<char>> &datasToSend)
{
    if (reg.hasComponent<ecs::component::SharedEntity>(entity)) {
        auto sharedId = reg.getComponent<ecs::component::SharedEntity>(entity)->sharedEntityId;
        datasToSend.push_back(rt::UDPPacket<rt::UDPBody::TAKE_DAMAGE>(
                                  rt::UDPCommand::TAKE_DAMAGE, sharedId, rt::UDPBody::TAKE_DAMAGE{.damage = damage}
        )
                                  .serialize());
    }
}

void rts::resolveTagEffect(
    ecs::Registry &reg,
    size_t entityA,
    size_t entityB,
    std::list<std::vector<char>> &datasToSend
) // Use tag here now !
{
    auto &missiles = reg.getComponents<ecs::component::Missile>();
    auto &health = reg.getComponents<ecs::component::Health>();

    if (missiles.has(entityA) && !missiles.has(entityB)) {
        if (health.has(entityB)) {
            tagEffectDamage(reg, entityB, 1, datasToSend);
            health[entityB]->currHp -= 1;
        } else {
            tagEffectKill(reg, entityB, datasToSend);
        }
        tagEffectKill(reg, entityA, datasToSend);
    }
    if (missiles.has(entityB) && !missiles.has(entityA)) {
        if (health.has(entityA)) {
            tagEffectDamage(reg, entityA, 1, datasToSend);
            health[entityA]->currHp -= 1;
        } else {
            tagEffectKill(reg, entityA, datasToSend);
        }
        tagEffectKill(reg, entityB, datasToSend);
    }
}

// NOLINTEND(bugprone-unchecked-optional-access)
