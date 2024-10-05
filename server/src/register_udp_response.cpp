/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** register_ecs
*/

#include <mutex>
#include <utility>
#include "RTypeServer.hpp"
#include "RTypeUDPProtol.hpp"
#include "components/position.hpp"
#include "components/velocity.hpp"

void rts::registerUdpResponse(
    ecs::Registry &reg,
    ntw::ResponseHandler<rt::UDPCommand, rt::UDPClientPacket> &responseHandler,
    std::list<rt::UDPServerPacket> &datasToSend,
    std::mutex &mut
)
{
    responseHandler.registerHandler(rt::UDPCommand::NEW_PLAYER, [&reg, &mut](const rt::UDPClientPacket &msg) {
        std::scoped_lock<std::mutex> lk(mut);
        rts::createPlayer(reg, msg.body.sharedEntityId);
    });
    responseHandler.registerHandler(rt::UDPCommand::MOVE_ENTITY, [&reg, &mut](const rt::UDPClientPacket &msg) {
        std::scoped_lock<std::mutex> lk(mut);

        try {
            reg.getComponent<ecs::component::Position>(reg.getLocalEntity().at(msg.body.sharedEntityId)).value() =
                msg.body.b.shareMovement.pos;
            reg.getComponent<ecs::component::Velocity>(reg.getLocalEntity().at(msg.body.sharedEntityId)).value() =
                msg.body.b.shareMovement.vel;
        } catch (...) {
        }
    });
    responseHandler.registerHandler(
        rt::UDPCommand::NEW_ENTITY,
        [&reg, &datasToSend, &mut](const rt::UDPClientPacket &msg) {
            std::scoped_lock<std::mutex> lk(mut);

            datasToSend.push_back(
                rt::UDPServerPacket({.header = {.cmd = rt::UDPCommand::NEW_ENTITY}, .body = std::move(msg.body)})
            );
            rts::createMissile(reg, msg);
        }
    );
}
