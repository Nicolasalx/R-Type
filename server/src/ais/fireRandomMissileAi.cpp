/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** fireRandomMissileAi
*/

#include "fireRandomMissileAi.hpp"
#include <cstdio>
#include <cstdlib>
#include <vector>
#include "RTypeUDPProtol.hpp"
#include "RandomGenerator.hpp"
#include "ServerEntityFactory.hpp"
#include "TimeoutHandler.hpp"
#include "components/hitbox.hpp"
#include "components/position.hpp"
#include "components/velocity.hpp"
#include "entity.hpp"
#include "shared_entity.hpp"

static ecs::component::Velocity generateEntityVel(float xFactor, float yFactor)
{
    auto velX = xFactor * 150;
    auto velY = yFactor * 150;

    if (velX < 150.f) {
        velX *= 100.f;
    }
    if (velY < 150.f) {
        velY *= 100.f;
    }
    return {velX, velY};
}

entity_t rts::ais::fireRandomMissileAi(
    ecs::Registry &reg,
    entity_t e,
    std::list<std::vector<char>> &datasToSend,
    ntw::TimeoutHandler &timeoutHandler,
    ntw::UDPServer &udpServer,
    int missileSpawnRate,
    const std::function<bool()> &cond,
    std::array<float, 2> randXRange,
    std::array<float, 2> randYRange
)
{
    if (cond != nullptr && !cond()) {
        return std::numeric_limits<size_t>::max();
    }
    if (eng::RandomGenerator::generate(1, missileSpawnRate) > 1) {
        return std::numeric_limits<size_t>::max();
    }
    auto aiPos = reg.getComponent<ecs::component::Position>(e);
    auto hitbox = reg.getComponent<ecs::component::Hitbox>(e);

    auto xFactor = eng::RandomGenerator::generate(randXRange[0], randXRange[1]);
    auto yFactor = eng::RandomGenerator::generate(randYRange[0], randYRange[1]);

    shared_entity_t sharedId = ecs::generateSharedEntityId();
    auto missilePosX = (aiPos->x - (hitbox->width / 2.f)) + xFactor * 20;
    auto missilePosY = (aiPos->y - (hitbox->height / 2.f)) + yFactor * 20;
    auto rMissile = ecs::ServerEntityFactory::createServerEntityFromJSON(
        reg, "assets/enemies/missileBall.json", missilePosX, missilePosY, sharedId, xFactor * 150, yFactor * 150
    );

    auto newMsg = rt::UDPPacket<rt::UDPBody::NEW_ENTITY_MISSILE_BALL>(
        rt::UDPCommand::NEW_ENTITY_MISSILE_BALL,
        sharedId,
        {.pos = {missilePosX, missilePosY}, .vel = generateEntityVel(xFactor, yFactor)},
        true
    );
    timeoutHandler.addTimeoutPacket(newMsg.serialize(), newMsg.packetId, udpServer);
    datasToSend.push_back(std::move(newMsg).serialize());
    return rMissile;
}
