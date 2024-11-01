/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** gravity_system
*/

#include "systems/gravity_system.hpp"
#include "RTypeUDPProtol.hpp"
#include "Registry.hpp"
#include "Zipper.hpp"
#include "components/gravity.hpp"
#include "components/position.hpp"
#include "components/velocity.hpp"

void ecs::systems::gravitySystem(Registry &reg, const sf::Vector2u &windowSize, ntw::UDPClient &udpClient)
{
    auto &gravities = reg.getComponents<ecs::component::Gravity>();
    auto &positions = reg.getComponents<ecs::component::Position>();
    auto &sharedEntity = reg.getComponents<ecs::component::SharedEntity>();
    auto &velocitys = reg.getComponents<ecs::component::Velocity>();

    ecs::Zipper<
        ecs::component::Gravity,
        ecs::component::Position,
        ecs::component::SharedEntity,
        ecs::component::Velocity>
        zipParallax(gravities, positions, sharedEntity, velocitys);

    for (auto [gravity, pos, sharedEnt, vel] : zipParallax) {
        if (gravity.ennemyType == "ground") {
            pos.y += 1.0f;

            rt::UDPPacket<rt::UDPBody::MOVE_ENTITY> msg(rt::UDPCommand::MOVE_ENTITY, sharedEnt.sharedEntityId);
            msg.body = {.pos = {pos.x, pos.y}, .vel = {.vx = vel.vx, .vy = vel.vy}};
            udpClient.send(reinterpret_cast<const char *>(&msg), sizeof(msg));
        }
    }
}
