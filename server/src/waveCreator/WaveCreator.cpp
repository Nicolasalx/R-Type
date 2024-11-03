/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** WaveCreator
*/

#include "WaveCreator.hpp"
#include <cstdio>
#include <fstream>
#include "RTypeUDPProtol.hpp"
#include "Registry.hpp"
#include "ServerEntityFactory.hpp"
#include "TrackedException.hpp"
#include "components/dobkeratops.hpp"
#include "entity.hpp"

#include "../ais/fireRandomMissileAi.hpp"
#include "../ais/horizontalMoveAi.hpp"
#include "../ais/splitAi.hpp"
#include "../ais/waveAi.hpp"
#include "../ais/dobkeratops_ai.hpp"
#include "components/ai_actor.hpp"
#include "components/death_split.hpp"
#include "nlohmann/json_fwd.hpp"

rts::WaveCreator::WaveCreator(const std::string &basePath) : _basePath(basePath)
{
    _initMobsFunctions();
}

rts::WaveCreator::WaveCreator(std::string &&basePath) : _basePath(std::move(basePath))
{
    _initMobsFunctions();
}

void rts::WaveCreator::createStage(
    size_t stage,
    int missileSpawnRate,
    ecs::WaveManager &waveManager,
    std::list<std::vector<char>> &datasToSend,
    ntw::UDPServer &udpServer,
    ntw::TimeoutHandler &timeoutHandler
)
{
    std::string jsonPath(std::format("{}/stage{}.json", _basePath, stage));

    std::ifstream file(jsonPath);
    if (!file.is_open()) {
        throw eng::TrackedException("Failed to open entity JSON file: " + jsonPath);
    }

    const nlohmann::json &waveJson = nlohmann::json::parse(file);

    if (!waveJson.contains("waves")) {
        throw eng::TrackedException("No waves specified in " + jsonPath);
    }

    for (const auto &[waveName, wave] : waveJson["waves"].items()) {
        auto waveId = waveManager.addNewWave();
        for (const auto &mobs : wave) {
            _addJSONMobs(waveId, mobs, missileSpawnRate, waveManager, datasToSend, udpServer, timeoutHandler);
        }
    }
}

void rts::WaveCreator::_addJSONMobs(
    size_t waveId,
    const nlohmann::json &mobs,
    int missileSpawnRate,
    ecs::WaveManager &waveManager,
    std::list<std::vector<char>> &datasToSend,
    ntw::UDPServer &udpServer,
    ntw::TimeoutHandler &timeoutHandler
)
{
    if (!mobs.contains("mobJsonPath") || !mobs.contains("pos") || !mobs.contains("vel") || !mobs.contains("type")) {
        return;
    }
    auto nbMob = mobs["nbMob"].get<int>();
    auto pos = mobs["pos"].begin();
    auto vel = mobs["vel"].begin();
    auto type = mobs["type"].begin();

    for (int i = 0; i < nbMob; i++) {
        float x = pos.value()["x"].get<float>();
        float y = pos.value()["y"].get<float>();

        float vx = vel.value()["vx"].get<float>();
        float vy = vel.value()["vy"].get<float>();

        waveManager.addNewMob(
            waveId,
            [mobJsonPath = mobs["mobJsonPath"].get<std::string>(),
             x,
             y,
             vx,
             vy,
             t = type->get<std::string>(),
             this,
             &datasToSend,
             missileSpawnRate,
             &waveManager,
             &timeoutHandler,
             &udpServer](ecs::Registry &reg) -> entity_t {
                shared_entity_t sharedEntityId = ecs::generateSharedEntityId();

                entity_t entity = ecs::ServerEntityFactory::createServerEntityFromJSON(
                    reg, mobJsonPath, x, y, sharedEntityId, vx, vy
                );
                _setupMobFunc[t](datasToSend, udpServer, timeoutHandler, reg, entity, sharedEntityId, x, y, vx, vy);
                reg.getComponent<ecs::component::AiActor>(entity)->act =
                    _mobAiCreateFunc[t](datasToSend, udpServer, timeoutHandler, waveManager, missileSpawnRate, x, y);
                return entity;
            }

        );
        pos++;
        vel++;
        type++;
    }
}

/**
 * Ais functions
 */

std::function<void(ecs::Registry &reg, entity_t e)> rts::WaveCreator::getDobkeratopsAi(
    std::list<std::vector<char>> &datasToSend,
    ntw::UDPServer &udpServer,
    ntw::TimeoutHandler &timeoutHandler,
    ecs::WaveManager & /*waveManager*/,
    int /*missileSpawnRate*/,
    float /*x*/,
    float /*y*/
)
{
    return [&datasToSend, &timeoutHandler, &udpServer](ecs::Registry &reg, entity_t e) {
        auto &stateComp = reg.getComponent<ecs::component::DobkeratopsState>(e);
        if (!stateComp->initialized) {
            rts::ais::initDobkeratopsAi(reg, e, datasToSend, timeoutHandler, udpServer, stateComp->state);
            stateComp->initialized = true;
        }
        rts::ais::dobkeratopsAi(reg, e, datasToSend, timeoutHandler, udpServer, stateComp->state);
    };
}

std::function<void(ecs::Registry &reg, entity_t e)> rts::WaveCreator::getRobotAi(
    std::list<std::vector<char>> &datasToSend,
    ntw::UDPServer &udpServer,
    ntw::TimeoutHandler &timeoutHandler,
    ecs::WaveManager & /*waveManager*/,
    int missileSpawnRate,
    float x,
    float /*y*/
)
{
    return [x, &datasToSend, missileSpawnRate, &udpServer, &timeoutHandler](ecs::Registry &reg, entity_t e) {
        rts::ais::horizontalMoveAi(reg, e, x - 100, x + 100);
        rts::ais::fireRandomMissileAi(
            reg, e, datasToSend, timeoutHandler, udpServer, missileSpawnRate, nullptr, {-1, 1}, {-1, 0}
        );
    };
}

std::function<void(ecs::Registry &reg, entity_t e)> rts::WaveCreator::getBydosWaveAi(
    std::list<std::vector<char>> &datasToSend,
    ntw::UDPServer &udpServer,
    ntw::TimeoutHandler &timeoutHandler,
    ecs::WaveManager & /*waveManager*/,
    int missileSpawnRate,
    float /*x*/,
    float y
)
{
    return [y, &datasToSend, missileSpawnRate, &timeoutHandler, &udpServer](ecs::Registry &reg, entity_t e) {
        rts::ais::waveMovement(reg, e, y);
        rts::ais::fireRandomMissileAi(reg, e, datasToSend, timeoutHandler, udpServer, missileSpawnRate);
    };
}

std::function<void(ecs::Registry &reg, entity_t e)> rts::WaveCreator::getBlobAi(
    std::list<std::vector<char>> &datasToSend,
    ntw::UDPServer &udpServer,
    ntw::TimeoutHandler &timeoutHandler,
    ecs::WaveManager &waveManager,
    int missileSpawnRate,
    float /*x*/,
    float y
)
{
    return [y, &datasToSend, missileSpawnRate, &waveManager, &timeoutHandler, &udpServer](ecs::Registry &r, entity_t entity) {
        rts::ais::waveMovement(r, entity, y);
        rts::ais::fireRandomMissileAi(r, entity, datasToSend, timeoutHandler, udpServer, missileSpawnRate);
        rts::ais::splitAi(r, entity, datasToSend, udpServer, timeoutHandler, waveManager);
    };
}

/**
 * Setup mob functions
 */

void rts::WaveCreator::setupBydosDatas(
    std::list<std::vector<char>> &datasToSend,
    ntw::UDPServer &udpServer,
    ntw::TimeoutHandler &timeoutHandler,
    ecs::Registry & /*reg*/,
    entity_t /*e*/,
    size_t sharedId,
    float x,
    float y,
    float /*vx*/,
    float /*vy*/
)
{
    auto newMsg = rt::UDPPacket<rt::UDPBody::NEW_ENTITY_BYDOS_WAVE>(
        rt::UDPCommand::NEW_ENTITY_BYDOS_WAVE, sharedId, {.pos = {x, y}}, true
    );
    timeoutHandler.addTimeoutPacket(newMsg.serialize(), newMsg.packetId, udpServer);
    datasToSend.push_back(std::move(newMsg).serialize());
}

void rts::WaveCreator::setupRobotDatas(
    std::list<std::vector<char>> &datasToSend,
    ntw::UDPServer &udpServer,
    ntw::TimeoutHandler &timeoutHandler,
    ecs::Registry & /*reg*/,
    entity_t /*e*/,
    size_t sharedId,
    float x,
    float y,
    float vx,
    float vy
)
{
    auto newMsg = rt::UDPPacket<rt::UDPBody::NEW_ENTITY_ROBOT_GROUND>(
        rt::UDPCommand::NEW_ENTITY_ROBOT_GROUND, sharedId, {.pos = {x, y}, .vel = {vx, vy}}, true
    );
    timeoutHandler.addTimeoutPacket(newMsg.serialize(), newMsg.packetId, udpServer);
    datasToSend.push_back(std::move(newMsg).serialize());
}

void rts::WaveCreator::setupDobkeratopsDatas(
    std::list<std::vector<char>> &datasToSend,
    ntw::UDPServer &udpServer,
    ntw::TimeoutHandler &timeoutHandler,
    ecs::Registry & /*reg*/,
    entity_t /*e*/,
    size_t sharedId,
    float x,
    float y,
    float /*vx*/,
    float /*vy*/
)
{
    auto newMsg = rt::UDPPacket<rt::UDPBody::NEW_ENTITY_DOBKERATOPS>(
        rt::UDPCommand::NEW_ENTITY_DOBKERATOPS, sharedId, {.pos = {x, y}, .stage = 1}, true
    );
    timeoutHandler.addTimeoutPacket(newMsg.serialize(), newMsg.packetId, udpServer);
    datasToSend.push_back(std::move(newMsg).serialize());
}

void rts::WaveCreator::setupBlobDatas(
    std::list<std::vector<char>> &datasToSend,
    ntw::UDPServer &udpServer,
    ntw::TimeoutHandler &timeoutHandler,
    ecs::Registry &reg,
    entity_t e,
    size_t sharedId,
    float x,
    float y,
    float /*vx*/,
    float /*vy*/
)
{
    reg.addComponent(e, ecs::component::DeathSplit{.splitCount = 2, .offsets = {{-32.0f, -32.0f}, {32.0f, -32.0f}}});
    auto newMsg = rt::UDPPacket<rt::UDPBody::NEW_ENTITY_BLOB>(
        rt::UDPCommand::NEW_ENTITY_BLOB, sharedId, {.pos = {.x = x, .y = y}}, true
    );
    timeoutHandler.addTimeoutPacket(newMsg.serialize(), newMsg.packetId, udpServer);
    datasToSend.push_back(std::move(newMsg).serialize());
}
