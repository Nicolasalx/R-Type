/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** init_waves
*/

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include "RTypeConst.hpp"
#include "RTypeServer.hpp"
#include "RTypeUDPProtol.hpp"
#include "Registry.hpp"
#include "ServerEntityFactory.hpp"
#include "WaveManager.hpp"
#include "ais/fireRandomMissileAi.hpp"
#include "ais/horizontalMoveAi.hpp"
#include "ais/splitAi.hpp"
#include "ais/waveAi.hpp"
#include "components/dobkeratops.hpp"
#include "entity.hpp"
#include "ais/dobkeratops_ai.hpp"
#include "components/ai_actor.hpp"
#include "components/death_split.hpp"
#include "shared_entity.hpp"

static entity_t spawnRobot(
    ecs::Registry &reg,
    std::list<std::vector<char>> &datasToSend,
    size_t maxMob,
    size_t i,
    int missileSpawnRate
)
{
    float x = float(rt::SCREEN_WIDTH) * (float(i) / float(maxMob + 1));
    float y = float(rt::SCREEN_HEIGHT) * (4.f / 5.f);
    auto sharedId = ecs::generateSharedEntityId();
    datasToSend.push_back(rt::UDPPacket<rt::UDPBody::NEW_ENTITY_ROBOT_GROUND>(
                              rt::UDPCommand::NEW_ENTITY_ROBOT_GROUND, sharedId, {.pos = {x, y}, .vel = {-10, 0}}, true
    )
                              .serialize());
    auto e = ecs::ServerEntityFactory::createServerEntityFromJSON(reg, "assets/robotGround.json", x, y, sharedId, -10);
    reg.getComponent<ecs::component::AiActor>(e)->act =
        [x, &datasToSend, missileSpawnRate](ecs::Registry &reg, entity_t e) {
            rts::ais::horizontalMoveAi(reg, e, x - 100, x + 100);
            rts::ais::fireRandomMissileAi(reg, e, datasToSend, missileSpawnRate, nullptr, {-1, 1}, {-1, 0});
        };
    return e;
}

static entity_t spawnBydosWaveSplit(
    ecs::Registry &reg,
    std::list<std::vector<char>> &datasToSend,
    size_t maxMob,
    size_t i,
    int missileSpawnRate,
    ecs::WaveManager &waveManager
)
{
    float x = float(rt::SCREEN_WIDTH) * (4.f / 5.f);
    float y = float(rt::SCREEN_HEIGHT) * (float(i) / float(maxMob + 1));
    auto sharedId = ecs::generateSharedEntityId();

    auto e = ecs::ServerEntityFactory::createServerEntityFromJSON(reg, "assets/blob.json", x, y, sharedId, -10.0f);

    reg.addComponent(e, ecs::component::DeathSplit{.splitCount = 2, .offsets = {{-32.0f, -32.0f}, {32.0f, -32.0f}}});
    auto &aiActor = reg.addComponent(e, ecs::component::AiActor{});
    aiActor->act = [y, &datasToSend, missileSpawnRate, &waveManager](ecs::Registry &r, entity_t entity) {
        rts::ais::waveMovement(r, entity, y);
        rts::ais::fireRandomMissileAi(r, entity, datasToSend, missileSpawnRate);
        rts::ais::splitAi(r, entity, datasToSend, waveManager);
    };

    datasToSend.push_back(rt::UDPPacket<rt::UDPBody::NEW_ENTITY_BLOB>(
                              rt::UDPCommand::NEW_ENTITY_BLOB, sharedId, {.pos = {.x = x, .y = y}}, true
    )
                              .serialize());

    return e;
}

static entity_t spawnBydosWave(
    ecs::Registry &reg,
    std::list<std::vector<char>> &datasToSend,
    size_t maxMob,
    size_t i,
    int missileSpawnRate
)
{
    float x = float(rt::SCREEN_WIDTH) * (4.f / 5.f);
    float y = float(rt::SCREEN_HEIGHT) * (float(i) / float(maxMob + 1));
    auto sharedId = ecs::generateSharedEntityId();
    datasToSend.push_back(rt::UDPPacket<rt::UDPBody::NEW_ENTITY_BYDOS_WAVE>(
                              rt::UDPCommand::NEW_ENTITY_BYDOS_WAVE, sharedId, {.pos = {x, y}}, true
    )
                              .serialize());
    auto e = ecs::ServerEntityFactory::createServerEntityFromJSON(reg, "assets/bydosWave.json", x, y, sharedId, -10);
    reg.getComponent<ecs::component::AiActor>(e)->act =
        [y, &datasToSend, missileSpawnRate](ecs::Registry &reg, entity_t e) {
            rts::ais::waveMovement(reg, e, y);
            rts::ais::fireRandomMissileAi(reg, e, datasToSend, missileSpawnRate);
        };
    return e;
}

static void waveInit(
    ecs::WaveManager &waveManager,
    std::list<std::vector<char>> &datasToSend,
    size_t maxMob,
    int missileSpawnRate
)
{
    auto waveId = waveManager.addNewWave();

    for (size_t i = 1; i < maxMob + 1; i++) {
        if (i % 2 == 0) {
            waveManager.addNewMob(waveId, [i, &datasToSend, maxMob, missileSpawnRate](ecs::Registry &reg) {
                return spawnRobot(reg, datasToSend, maxMob, i, missileSpawnRate);
            });
        } else {
            waveManager.addNewMob(waveId, [i, &datasToSend, maxMob, missileSpawnRate](ecs::Registry &reg) {
                return spawnBydosWave(reg, datasToSend, maxMob, i, missileSpawnRate);
            });
        }
    }
}

static void waveInitSplit(
    ecs::WaveManager &waveManager,
    std::list<std::vector<char>> &datasToSend,
    size_t maxMob,
    int missileSpawnRate
)
{
    auto waveId = waveManager.addNewWave();

    for (size_t i = 1; i < maxMob + 1; i++) {
        if (i % 2 == 0) {
            waveManager.addNewMob(waveId, [i, &datasToSend, maxMob, missileSpawnRate](ecs::Registry &reg) {
                return spawnRobot(reg, datasToSend, maxMob, i, missileSpawnRate);
            });
        } else {
            waveManager.addNewMob(
                waveId,
                [i, &datasToSend, maxMob, missileSpawnRate, &waveManager](ecs::Registry &reg) {
                    return spawnBydosWaveSplit(reg, datasToSend, maxMob, i, missileSpawnRate, waveManager);
                }
            );
        }
    }
}

void rts::initWaves(ecs::WaveManager &waveManager, std::list<std::vector<char>> &datasToSend, int missileSpawnRate)
{
    waveInit(waveManager, datasToSend, 5, missileSpawnRate);
    waveInitSplit(waveManager, datasToSend, 3, missileSpawnRate);
    waveInit(waveManager, datasToSend, 6, missileSpawnRate);
    auto bossWaveId = waveManager.addNewWave();
    waveManager.addNewMob(bossWaveId, [&datasToSend](ecs::Registry &reg) -> entity_t {
        shared_entity_t sharedEntityId = ecs::generateSharedEntityId();
        entity_t entity = ecs::ServerEntityFactory::createServerEntityFromJSON(
            reg, "assets/dobkeratops.json", 560.f, 120.f, sharedEntityId
        );
        reg.addComponent<ecs::component::DobkeratopsState>(entity, ecs::component::DobkeratopsState{});
        datasToSend.push_back(
            rt::UDPPacket<rt::UDPBody::NEW_ENTITY_DOBKERATOPS>(
                rt::UDPCommand::NEW_ENTITY_DOBKERATOPS, sharedEntityId, {.pos = {560.f, 120.f}, .stage = 1}, true
            )
                .serialize()
        );

        reg.getComponent<ecs::component::AiActor>(entity)->act = [&datasToSend](ecs::Registry &reg, entity_t e) {
            auto &stateComp = reg.getComponent<ecs::component::DobkeratopsState>(e);
            if (!stateComp->initialized) {
                rts::ais::initDobkeratopsAi(reg, e, datasToSend, stateComp->state);
                stateComp->initialized = true;
            }
            rts::ais::dobkeratopsAi(reg, e, datasToSend, stateComp->state);
        };

        return entity;
    });

    waveManager.addNewMob(bossWaveId, [&datasToSend, &missileSpawnRate](ecs::Registry &reg) -> entity_t {
        shared_entity_t sharedEntityId = ecs::generateSharedEntityId();
        entity_t entity = ecs::ServerEntityFactory::createServerEntityFromJSON(
            reg, "assets/bydosWave.json", 500.f, 100.f, sharedEntityId
        );

        datasToSend.push_back(rt::UDPPacket<rt::UDPBody::NEW_ENTITY_BYDOS_WAVE>(
                                  rt::UDPCommand::NEW_ENTITY_BYDOS_WAVE, sharedEntityId, {.pos = {500.f, 100.f}}, true
        )
                                  .serialize());

        reg.getComponent<ecs::component::AiActor>(entity)->act = [&datasToSend,
                                                                  &missileSpawnRate](ecs::Registry &reg, entity_t e) {
            rts::ais::waveMovement(reg, e, 100.f);
            rts::ais::fireRandomMissileAi(reg, e, datasToSend, missileSpawnRate);
        };

        return entity;
    });

    waveManager.addNewMob(bossWaveId, [&datasToSend, &missileSpawnRate](ecs::Registry &reg) -> entity_t {
        shared_entity_t sharedEntityId = ecs::generateSharedEntityId();
        entity_t entity = ecs::ServerEntityFactory::createServerEntityFromJSON(
            reg, "assets/bydosWave.json", 500.f, 380.f, sharedEntityId
        );

        datasToSend.push_back(rt::UDPPacket<rt::UDPBody::NEW_ENTITY_BYDOS_WAVE>(
                                  rt::UDPCommand::NEW_ENTITY_BYDOS_WAVE, sharedEntityId, {.pos = {500.f, 380.f}}, true
        )
                                  .serialize());

        reg.getComponent<ecs::component::AiActor>(entity)->act = [&datasToSend,
                                                                  &missileSpawnRate](ecs::Registry &reg, entity_t e) {
            rts::ais::waveMovement(reg, e, 380.f);
            rts::ais::fireRandomMissileAi(reg, e, datasToSend, missileSpawnRate);
        };

        return entity;
    });
}
