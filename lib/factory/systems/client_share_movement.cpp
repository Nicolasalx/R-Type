/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** position
*/

#include "components/client_share_movement.hpp"
#include "RTypeUDPProtol.hpp"
#include "Registry.hpp"
#include "Zipper.hpp"
#include "components/position.hpp"
#include "components/velocity.hpp"
#include "udp/UDPClient.hpp"
#include "components/shared_entity.hpp"

namespace ecs::systems {

void clientShareMovement(Registry &reg, ntw::UDPClient &udpClient)
{
    auto &sharedMov = reg.getComponents<ecs::component::ClientShareMovement>();
    auto &positions = reg.getComponents<ecs::component::Position>();
    auto &velocitys = reg.getComponents<ecs::component::Velocity>();
    auto &sharedEntity = reg.getComponents<ecs::component::SharedEntity>();

    Zipper<
        ecs::component::ClientShareMovement,
        ecs::component::Position,
        ecs::component::Velocity,
        ecs::component::SharedEntity>
        zip(sharedMov, positions, velocitys, sharedEntity);

    for (auto [_, pos, vel, shared_entity] : zip) {
        rt::UDPPacket<rt::UDPBody::MOVE_ENTITY> msg(rt::UDPCommand::MOVE_ENTITY, shared_entity.sharedEntityId);
        msg.body = {.pos = {pos.x, pos.y}, .vel = {.vx = vel.vx, .vy = vel.vy}};
        udpClient.send(reinterpret_cast<const char *>(&msg), sizeof(msg));
    }
}

} // namespace ecs::systems
