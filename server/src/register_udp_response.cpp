/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** register_ecs
*/

#include <cstddef>
#include <cstdio>
#include <exception>
#include <string>
#include <utility>
#include "RTypeServer.hpp"
#include "RTypeUDPProtol.hpp"
#include "Registry.hpp"
#include "ServerEntityFactory.hpp"
#include "components/player.hpp"
#include "components/position.hpp"
#include "components/velocity.hpp"

// dataToSend inside networkCallbacks
static void handlePlayerCreation(
    std::list<std::vector<char>> &datasToSend,
    eng::SafeList<std::function<void(ecs::Registry &reg)>> &networkCallbacks,
    const rt::UDPPacket<rt::UDPBody::NEW_ENTITY_PLAYER> &msg
)
{
    shared_entity_t sharedEntityId = msg.sharedEntityId;
    std::size_t playerIndex = msg.body.playerIndex;
    const auto &pos = msg.body.pos;

    networkCallbacks.pushBack([playerIndex, sharedEntityId, pos, msg, &datasToSend](ecs::Registry &reg) {
        auto entity = ecs::ServerEntityFactory::createServerEntityFromJSON(
            reg, "assets/player" + std::to_string(playerIndex) + ".json", pos.x, pos.y, sharedEntityId
        );
        reg.getComponent<ecs::component::Player>(entity)->id = msg.body.playerId;
        datasToSend.push_back(std::move(msg).serialize());
    });
}

static void handleMissileCreation(
    std::list<std::vector<char>> &datasToSend,
    eng::SafeList<std::function<void(ecs::Registry &reg)>> &networkCallbacks,
    const rt::UDPPacket<rt::UDPBody::NEW_ENTITY_MISSILE> &msg
)
{
    const auto &pos = msg.body.pos;
    const auto &vel = msg.body.vel;

    networkCallbacks.pushBack([pos, vel, msg, &datasToSend](ecs::Registry &reg) {
        ecs::ServerEntityFactory::createServerEntityFromJSON(
            reg, "assets/missile.json", pos.x, pos.y, msg.sharedEntityId, vel.vx, vel.vy
        );
        datasToSend.push_back(std::move(msg).serialize());
    });
}

void rts::registerUdpResponse(
    rt::UDPResponseHandler &responseHandler,
    std::list<std::vector<char>> &datasToSend,
    eng::SafeList<std::function<void(ecs::Registry &reg)>> &networkCallbacks,
    ntw::UDPServer &udpServer
)
{
    responseHandler.registerHandler<rt::UDPBody::NEW_ENTITY_PLAYER>(
        rt::UDPCommand::NEW_ENTITY_PLAYER,
        [&datasToSend, &networkCallbacks](const rt::UDPPacket<rt::UDPBody::NEW_ENTITY_PLAYER> &msg) {
            handlePlayerCreation(datasToSend, networkCallbacks, msg);
        }
    );
    responseHandler.registerHandler<rt::UDPBody::NEW_ENTITY_MISSILE>(
        rt::UDPCommand::NEW_ENTITY_MISSILE,
        [&datasToSend, &networkCallbacks](const rt::UDPPacket<rt::UDPBody::NEW_ENTITY_MISSILE> &msg) {
            handleMissileCreation(datasToSend, networkCallbacks, msg);
        }
    );
    responseHandler.registerHandler<rt::UDPBody::MOVE_ENTITY>(
        rt::UDPCommand::MOVE_ENTITY,
        [&networkCallbacks](const rt::UDPPacket<rt::UDPBody::MOVE_ENTITY> &msg) {
            networkCallbacks.pushBack([msg](ecs::Registry &reg) {
                try {
                    reg.getComponent<ecs::component::Position>(reg.getLocalEntity().at(msg.sharedEntityId)).value() =
                        msg.body.pos;
                    reg.getComponent<ecs::component::Velocity>(reg.getLocalEntity().at(msg.sharedEntityId)).value() =
                        msg.body.vel;
                } catch (const std::exception &e) {
                    // If entity does not exist, maybe server is late or ahead.
                    eng::logTimeWarning(e.what());
                }
            });
        }
    );
    responseHandler.registerHandler<rt::UDPBody::PING>(
        rt::UDPCommand::PING,
        [&udpServer](const rt::UDPPacket<rt::UDPBody::PING> &msg, const std::vector<std::any> &arg) {
            udpServer.send(
                std::any_cast<std::reference_wrapper<udp::endpoint>>(arg.at(0)).get(),
                reinterpret_cast<const char *>(&msg),
                msg.size
            );
        }
    );
}
