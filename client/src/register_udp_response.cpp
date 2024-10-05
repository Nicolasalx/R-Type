/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** register_udp_response
*/

#include "GameManager.hpp"
#include "RTypeClient.hpp"
#include "core/Registry.hpp"

void rtc::GameManager::_registerUdpResponse(ecs::Registry &reg, ecs::SpriteManager &spriteManager)
{
    _udpResponseHandler.registerHandler(
        rt::UDPCommand::NEW_ENTITY,
        [&spriteManager, &reg, this](const rt::UDPServerPacket &packet) {
            std::scoped_lock<std::mutex> lk(_mut);

            if (packet.body.b.newEntityData.type == 1) {
                auto &[pos, _] = packet.body.b.newEntityData.moveData;
                auto sharedEntityId = packet.body.sharedEntityId;

                rtc::createMissile(reg, spriteManager, sharedEntityId, pos.x, pos.y);
            }
            if (packet.body.b.newEntityData.type == 0) {
                auto &[pos, _] = packet.body.b.newEntityData.moveData;

                rtc::createStatic(reg, spriteManager, pos.x, pos.y);
            }
        }
    );
    _udpResponseHandler.registerHandler(rt::UDPCommand::MOVE_ENTITY, [&reg, this](const rt::UDPServerPacket &packet) {
        std::scoped_lock<std::mutex> lk(_mut);

        auto &sharedEntityId = packet.body.sharedEntityId;

        try {
            reg.getComponent<ecs::component::Position>(reg.getLocalEntity().at(sharedEntityId)).value() =
                packet.body.b.shareMovement.pos;
            reg.getComponent<ecs::component::Velocity>(reg.getLocalEntity().at(sharedEntityId)).value() =
                packet.body.b.shareMovement.vel;
        } catch (...) {
            // If entity does not exist, maybe server is late or ahead.
        }
    });
}
