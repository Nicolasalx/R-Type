#pragma once

#include <array>
#include <cmath>
#include <list>
#include <vector>
#include "Logger.hpp"
#include "RTypeUDPProtol.hpp"
#include "RandomGenerator.hpp"
#include "Registry.hpp"
#include "ServerEntityFactory.hpp"
#include "components/animation.hpp"
#include "components/health.hpp"
#include "components/position.hpp"
#include "components/velocity.hpp"
#include "components/shared_entity.hpp"

namespace rts::ais {

struct DobkeratopsState {
    int attackTicks = 0;
    int neckTicks = 0;
    bool isAttacking = false;
    std::vector<entity_t> neckSegments;
    float targetY = 240.0f;
    int attackPattern = 0;
    int projectilesShot = 0;
};

static std::unordered_map<entity_t, DobkeratopsState> dobkeratopsStates;

static void sendAnimationStateChange(
    std::list<std::vector<char>> &datasToSend,
    shared_entity_t entityId,
    const std::string &newState
)
{
    rt::UDPPacket<rt::UDPBody::CHANGE_ANIMATION_STATE> msg(rt::UDPCommand::CHANGE_ANIMATION_STATE, entityId);
    newState.copy(msg.body.newState, sizeof(msg.body.newState) - 1);
    datasToSend.push_back(msg.serialize());
}

entity_t createNeckSegment(
    ecs::Registry &reg,
    float x,
    float y,
    int partIndex,
    std::list<std::vector<char>> &datasToSend
)
{
    shared_entity_t sharedId = ecs::generateSharedEntityId();

    entity_t segment =
        ecs::ServerEntityFactory::createServerEntityFromJSON(reg, "assets/dobkeratops_segment.json", x, y, sharedId);

    datasToSend.push_back(
        rt::UDPPacket<rt::UDPBody::NEW_ENTITY_DOBKERATOPS_PART>(
            rt::UDPCommand::NEW_ENTITY_DOBKERATOPS_PART, sharedId, {.pos = {x, y}, .partIndex = partIndex}
        )
            .serialize()
    );

    return segment;
}

void initDobkeratopsAi(ecs::Registry &reg, entity_t e, std::list<std::vector<char>> &datasToSend)
{
    auto &state = dobkeratopsStates[e];
    auto pos = reg.getComponent<ecs::component::Position>(e);
    if (!pos) {
        eng::logWarning("Failed to get position component during init");
        return;
    }

    const int numSegments = 8;
    float segmentSpacing = 16.0f;

    for (int i = 0; i < numSegments; i++) {
        float segX = pos->x - (i + 1) * segmentSpacing + 60.0f;
        float segY = pos->y + std::sin(i * 0.5f) * 20.0f + 180.0f;

        entity_t segment = createNeckSegment(reg, segX, segY, i, datasToSend);
        state.neckSegments.push_back(segment);
        eng::logInfo("Created segment with entity ID: " + std::to_string(segment));
    }

    state.targetY = pos->y;
}

void spawnDobkeratopsProjectile(
    ecs::Registry &reg,
    entity_t bossEntity,
    std::list<std::vector<char>> &datasToSend,
    float angle
)
{
    auto bossPos = reg.getComponent<ecs::component::Position>(bossEntity);
    if (!bossPos) {
        return;
    }

    const float projectileSpeed = 250.0f;
    shared_entity_t sharedId = ecs::generateSharedEntityId();

    float vx = -projectileSpeed * std::cos(angle);
    float vy = projectileSpeed * std::sin(angle);

    float spawnX = bossPos->x - 40.0f;
    float spawnY = bossPos->y + 60.0f;

    ecs::ServerEntityFactory::createServerEntityFromJSON(
        reg, "assets/boss_projectile.json", spawnX, spawnY, sharedId, vx, vy
    );

    datasToSend.push_back(
        rt::UDPPacket<rt::UDPBody::NEW_ENTITY_MISSILE_BALL>(
            rt::UDPCommand::NEW_ENTITY_MISSILE_BALL, sharedId, {.pos = {spawnX, spawnY}, .vel = {vx, vy}}
        ).serialize()
    );
}

void cleanupDobkeratopsSegments(ecs::Registry &reg, entity_t bossEntity, std::list<std::vector<char>> &datasToSend)
{
    if (!dobkeratopsStates.contains(bossEntity)) {
        eng::logWarning("Attempted to cleanup non-existent Dobkeratops state");
        return;
    }

    auto &state = dobkeratopsStates[bossEntity];

    std::vector<entity_t> segmentsToRemove = state.neckSegments;

    state.neckSegments.clear();

    for (auto segment : segmentsToRemove) {
        try {
            if (!reg.hasComponent<ecs::component::SharedEntity>(segment)) {
                eng::logWarning("Segment " + std::to_string(segment) + " missing SharedEntity component");
                reg.killEntity(segment);
                continue;
            }

            auto sharedId = reg.getComponent<ecs::component::SharedEntity>(segment)->sharedEntityId;

            datasToSend.push_back(
                rt::UDPPacket<rt::UDPBody::DEL_ENTITY>(rt::UDPCommand::DEL_ENTITY, sharedId).serialize()
            );
            reg.killEntity(segment);

        } catch (const std::exception &e) {
            eng::logError("Failed to cleanup segment " + std::to_string(segment) + ": " + e.what());
        }
    }
    dobkeratopsStates.erase(bossEntity);
}

void dobkeratopsAi(ecs::Registry &reg, entity_t e, std::list<std::vector<char>> &datasToSend)
{
    auto &state = dobkeratopsStates[e];
    auto bossPos = reg.getComponent<ecs::component::Position>(e);
    auto vel = reg.getComponent<ecs::component::Velocity>(e);
    auto health = reg.getComponent<ecs::component::Health>(e);
    auto anim = reg.getComponent<ecs::component::Animation>(e);
    auto sharedEntity = reg.getComponent<ecs::component::SharedEntity>(e);

    if (!bossPos || !vel || !health || !anim || !sharedEntity) {
        eng::logWarning("Missing required components for Dobkeratops AI");
        return;
    }

    state.attackTicks++;
    state.neckTicks++;

    const float maxSegmentDistance = 20.0f;
    const float segmentLength = 16.0f;
    const float initialXOffset = 58.0f;
    const float initialYOffset = 180.0f;

    for (size_t i = 0; i < state.neckSegments.size(); i++) {
        entity_t segmentEntity = state.neckSegments[i];
        if (!reg.hasComponent<ecs::component::Position>(segmentEntity)) {
            continue;
        }

        auto &segPos = reg.getComponent<ecs::component::Position>(segmentEntity);

        if (i == 0) {
            float targetX = bossPos->x + initialXOffset;
            float targetY = bossPos->y + initialYOffset;

            segPos->x = targetX;
            segPos->y = targetY;
        } else {
            auto prevSegPos = reg.getComponent<ecs::component::Position>(state.neckSegments[i - 1]);
            if (!prevSegPos) {
                continue;
            }

            float targetX = prevSegPos->x - segmentLength;
            float baseY = prevSegPos->y - (segmentLength * 0.2f);

            float amplitude = std::min(6.0f * i, 25.0f);
            float delay = i * 0.3f;
            float targetY = baseY + std::sin(state.neckTicks * 0.05f - delay) * amplitude;

            float dx = targetX - prevSegPos->x;
            float dy = targetY - prevSegPos->y;
            float distance = std::sqrt(dx * dx + dy * dy);
            if (distance > maxSegmentDistance) {
                float ratio = maxSegmentDistance / distance;
                targetX = prevSegPos->x + dx * ratio;
                targetY = prevSegPos->y + dy * ratio;
            }

            segPos->x = targetX;
            segPos->y = targetY;
        }
    }

    if (state.attackTicks >= 90) {
        state.attackTicks = 0;
        state.isAttacking = true;
        state.projectilesShot = 0;
        state.attackPattern = (state.attackPattern + 1) % 3;
        anim->state = "attack";
        sendAnimationStateChange(datasToSend, sharedEntity->sharedEntityId, "attack");
    }

    if (state.isAttacking) {
        switch (state.attackPattern) {
            case 0:
                if (state.projectilesShot < 1) {
                    float angle = eng::RandomGenerator::generate(-0.5f, 0.5f);
                    spawnDobkeratopsProjectile(reg, e, datasToSend, angle);
                    state.projectilesShot++;
                }
                break;

            case 1:
                if (state.projectilesShot < 8) {
                    float baseAngle = -0.5f;
                    float angleStep = 0.2f;
                    spawnDobkeratopsProjectile(reg, e, datasToSend, baseAngle + angleStep * state.projectilesShot);
                    state.projectilesShot++;
                }
                break;

            case 2:
                if (state.projectilesShot < 20 && (state.attackTicks % 3 == 0)) {
                    float angle = eng::RandomGenerator::generate(-0.7f, 0.7f);
                    spawnDobkeratopsProjectile(reg, e, datasToSend, angle);
                    state.projectilesShot++;
                }
                break;
        }

        if ((state.attackPattern != 2 && state.projectilesShot >= 8) ||
            (state.attackPattern == 2 && state.projectilesShot >= 20)) {
            state.isAttacking = false;
            anim->state = "idle";
            sendAnimationStateChange(datasToSend, sharedEntity->sharedEntityId, "idle");
        }
    }

    if (health->currHp <= 1) {
        cleanupDobkeratopsSegments(reg, e, datasToSend);
        datasToSend.push_back(
            rt::UDPPacket<rt::UDPBody::DEL_ENTITY>(rt::UDPCommand::DEL_ENTITY, sharedEntity->sharedEntityId).serialize()
        );
        reg.killEntity(e);
        return;
    }
}

} // namespace rts::ais
