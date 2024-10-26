/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** register_ecs
*/

#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <exception>
#include <mutex>
#include <string>
#include <sys/types.h>
#include <utility>
#include "Logger.hpp"
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
    const rt::UDPPacket<rt::UDPBody::NEW_ENTITY_PLAYER> &msg,
    ntw::TimeoutHandler &timeoutHandler,
    ntw::UDPServer &udpServer
)
{
    auto newMsg = msg;
    shared_entity_t sharedEntityId = newMsg.sharedEntityId;
    std::size_t playerIndex = newMsg.body.playerIndex;
    const auto &pos = newMsg.body.pos;

    newMsg.ack = true;
    networkCallbacks.pushBack(
        [playerIndex, sharedEntityId, pos, newMsg, &datasToSend, &udpServer, &timeoutHandler](ecs::Registry &reg) {
            auto entity = ecs::ServerEntityFactory::createServerEntityFromJSON(
                reg, "assets/player" + std::to_string(playerIndex) + ".json", pos.x, pos.y, sharedEntityId
            );
            reg.getComponent<ecs::component::Player>(entity)->id = newMsg.body.playerId;
            timeoutHandler.addTimeoutPacket(newMsg.serialize(), newMsg.packetId, udpServer);
            datasToSend.push_back(std::move(newMsg).serialize());
        }
    );
}

static void handleMissileCreation(
    std::list<std::vector<char>> &datasToSend,
    eng::SafeList<std::function<void(ecs::Registry &reg)>> &networkCallbacks,
    const rt::UDPPacket<rt::UDPBody::NEW_ENTITY_MISSILE> &msg,
    ntw::TimeoutHandler &timeoutHandler,
    ntw::UDPServer &udpServer
)
{
    auto newMsg = msg;
    const auto &pos = newMsg.body.pos;
    const auto &vel = newMsg.body.vel;

    newMsg.ack = true;
    networkCallbacks.pushBack([pos, vel, newMsg, &datasToSend, &timeoutHandler, &udpServer](ecs::Registry &reg) {
        ecs::ServerEntityFactory::createServerEntityFromJSON(
            reg, "assets/missile.json", pos.x, pos.y, newMsg.sharedEntityId, vel.vx, vel.vy
        );
        timeoutHandler.addTimeoutPacket(newMsg.serialize(), newMsg.packetId, udpServer);
        datasToSend.push_back(std::move(newMsg).serialize());
    });
}

static void handleAckClient(
    ntw::UDPServer &udpServer,
    const std::vector<std::any> &arg,
    size_t packetId,
    u_int8_t cmd,
    ntw::TimeoutHandler &timeoutHandler,
    std::atomic<bool> &stopGame
)
{
    auto &mut = udpServer.mut();
    std::lock_guard<std::recursive_mutex> lck(mut);

    auto &end = std::any_cast<std::reference_wrapper<udp::endpoint>>(arg.at(0)).get();
    auto &allEndpoints = udpServer.endpoints();
    auto it = std::find_if(allEndpoints.begin(), allEndpoints.end(), [&end](auto &val) { return val.second == end; });
    if (it == allEndpoints.end()) {
        return;
    }
    if (!timeoutHandler.removeClient(packetId, it->first)) {
        return;
    }
    if ((rt::UDPCommand)cmd == rt::UDPCommand::END_GAME) {
        stopGame.store(true);
    }
}

void rts::registerUdpResponse(
    rt::UDPResponseHandler &responseHandler,
    std::list<std::vector<char>> &datasToSend,
    eng::SafeList<std::function<void(ecs::Registry &reg)>> &networkCallbacks,
    ntw::UDPServer &udpServer,
    ntw::TimeoutHandler &timeoutHandler,
    std::atomic<bool> &stopGame
)
{
    responseHandler.registerAckHandler([&udpServer, &networkCallbacks, &timeoutHandler, &stopGame](
                                           u_int8_t cmd, size_t packetId, const std::vector<std::any> &arg
                                       ) {
        networkCallbacks.pushBack([&timeoutHandler, arg, &udpServer, packetId, cmd, &stopGame](ecs::Registry &) {
            handleAckClient(udpServer, arg, packetId, cmd, timeoutHandler, stopGame);
        });
    });
    responseHandler.registerHandler<rt::UDPBody::NEW_ENTITY_PLAYER>(
        rt::UDPCommand::NEW_ENTITY_PLAYER,
        [&datasToSend, &networkCallbacks, &timeoutHandler, &udpServer](
            const rt::UDPPacket<rt::UDPBody::NEW_ENTITY_PLAYER> &msg
        ) { handlePlayerCreation(datasToSend, networkCallbacks, msg, timeoutHandler, udpServer); }
    );
    responseHandler.registerHandler<rt::UDPBody::NEW_ENTITY_MISSILE>(
        rt::UDPCommand::NEW_ENTITY_MISSILE,
        [&datasToSend, &networkCallbacks, &timeoutHandler, &udpServer](
            const rt::UDPPacket<rt::UDPBody::NEW_ENTITY_MISSILE> &msg
        ) { handleMissileCreation(datasToSend, networkCallbacks, msg, timeoutHandler, udpServer); }
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
