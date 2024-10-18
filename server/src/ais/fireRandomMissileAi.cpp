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
#include "components/hitbox.hpp"
#include "components/position.hpp"
#include "entity.hpp"
#include "shared_entity.hpp"

entity_t rts::ais::fireRandomMissileAi(
    ecs::Registry &reg,
    entity_t e,
    std::list<std::vector<char>> &datasToSend,
    const std::function<bool()> &cond,
    std::array<float, 2> randXRange,
    std::array<float, 2> randYRange
)
{
    if (cond != nullptr && !cond()) {
        return std::numeric_limits<size_t>::max();
    }
    if (eng::RandomGenerator::generate(0, 100) > 0) {
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
        reg, "assets/missileBall.json", missilePosX, missilePosY, sharedId, xFactor * 150, yFactor * 150
    );

    datasToSend.push_back(rt::UDPPacket<rt::UDPBody::NEW_ENTITY_MISSILE_BALL>(
                              rt::UDPCommand::NEW_ENTITY_MISSILE_BALL,
                              sharedId,
                              {.moveData = {.pos = {missilePosX, missilePosY}, .vel = {xFactor * 150, yFactor * 150}}}
    ).serialize());
    return rMissile;
}
