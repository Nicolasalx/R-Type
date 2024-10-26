/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** register_udp_response
*/

#include <cstddef>
#include <cstdio>
#include <exception>
#include <string>
#include "ClientEntityFactory.hpp"
#include "GameManager.hpp"
#include "Logger.hpp"
#include "RTypeUDPProtol.hpp"
#include "Registry.hpp"
#include "SpriteManager.hpp"
#include "Zipper.hpp"
#include "components/controllable.hpp"
#include "components/player.hpp"
#include "components/score.hpp"
#include "components/velocity.hpp"
#include "imgui.h"
#include "components/ally_player.hpp"
#include "components/client_share_movement.hpp"
#include "components/on_death.hpp"
#include "components/score_earned.hpp"
#include "components/self_player.hpp"
#include <imgui-SFML.h>

static void handlePlayerCreation(
    std::size_t userId,
    ecs::SpriteManager &spriteManager,
    eng::SafeList<std::function<void(ecs::Registry &)>> &networkCallbacks,
    const rt::UDPPacket<rt::UDPBody::NEW_ENTITY_PLAYER> &packet,
    const std::shared_ptr<ImFont> &font
)
{
    networkCallbacks.pushBack([packet, &spriteManager, userId, font](ecs::Registry &reg) {
        auto entity = ecs::ClientEntityFactory::createClientEntityFromJSON(
            reg,
            spriteManager,

            "assets/player" + std::to_string(packet.body.playerIndex) + ".json",
            packet.body.pos.x,
            packet.body.pos.y,
            packet.sharedEntityId,
            0.0f,
            0.0f,
            font
        );
        reg.getComponent<ecs::component::Player>(entity) =
            ecs::component::Player{.name = packet.body.playerName, .id = userId};
        if (packet.body.playerId == userId) {
            reg.addComponent<ecs::component::SelfPlayer>(entity, ecs::component::SelfPlayer{});
        } else {
            reg.addComponent<ecs::component::AllyPlayer>(entity, ecs::component::AllyPlayer{});
            reg.removeComponent<ecs::component::Controllable>(reg.getLocalEntity().at(packet.sharedEntityId));
            reg.removeComponent<ecs::component::ClientShareMovement>(reg.getLocalEntity().at(packet.sharedEntityId));
        }
    });
}

template <typename T>
static void handleSharedCreation(
    const std::string &jsonFilePath,
    ecs::SpriteManager &spriteManager,
    eng::SafeList<std::function<void(ecs::Registry &)>> &networkCallbacks,
    const rt::UDPPacket<T> &packet
)
{
    auto &pos = packet.body.pos;
    auto sharedEntityId = packet.sharedEntityId;

    networkCallbacks.pushBack([sharedEntityId, pos, &spriteManager, jsonFilePath](ecs::Registry &reg) {
        ecs::ClientEntityFactory::createClientEntityFromJSON(
            reg, spriteManager, jsonFilePath, pos.x, pos.y, sharedEntityId
        );
    });
}

void rtc::GameManager::_registerUdpResponse(ecs::SpriteManager &spriteManager)
{
    _udpResponseHandler.registerHandler<rt::UDPBody::NEW_ENTITY_STATIC>(
        rt::UDPCommand::NEW_ENTITY_STATIC,
        [this, &spriteManager](const rt::UDPPacket<rt::UDPBody::NEW_ENTITY_STATIC> &packet) {
            handleSharedCreation<rt::UDPBody::NEW_ENTITY_STATIC>(
                "assets/static.json", spriteManager, this->_networkCallbacks, packet
            );
        }
    );
    _udpResponseHandler.registerHandler<rt::UDPBody::NEW_ENTITY_PLAYER>(
        rt::UDPCommand::NEW_ENTITY_PLAYER,
        [this, &spriteManager](const rt::UDPPacket<rt::UDPBody::NEW_ENTITY_PLAYER> &packet) {
            handlePlayerCreation(_userId, spriteManager, this->_networkCallbacks, packet, _font);
        }
    );
    _udpResponseHandler.registerHandler<rt::UDPBody::NEW_ENTITY_MISSILE>(
        rt::UDPCommand::NEW_ENTITY_MISSILE,
        [this, &spriteManager](const rt::UDPPacket<rt::UDPBody::NEW_ENTITY_MISSILE> &packet) {
            handleSharedCreation<rt::UDPBody::NEW_ENTITY_MISSILE>(
                "assets/missile.json", spriteManager, this->_networkCallbacks, packet
            );
        }
    );
    _udpResponseHandler.registerHandler<rt::UDPBody::NEW_ENTITY_MISSILE_BALL>(
        rt::UDPCommand::NEW_ENTITY_MISSILE_BALL,
        [this, &spriteManager](const rt::UDPPacket<rt::UDPBody::NEW_ENTITY_MISSILE_BALL> &packet) {
            handleSharedCreation<rt::UDPBody::NEW_ENTITY_MISSILE_BALL>(
                "assets/missileBall.json", spriteManager, this->_networkCallbacks, packet
            );
        }
    );
    _udpResponseHandler.registerHandler<rt::UDPBody::NEW_ENTITY_BYDOS_WAVE>(
        rt::UDPCommand::NEW_ENTITY_BYDOS_WAVE,
        [this, &spriteManager](const rt::UDPPacket<rt::UDPBody::NEW_ENTITY_BYDOS_WAVE> &packet) {
            handleSharedCreation<rt::UDPBody::NEW_ENTITY_BYDOS_WAVE>(
                "assets/bydosWave.json", spriteManager, this->_networkCallbacks, packet
            );
        }
    );
    _udpResponseHandler.registerHandler<rt::UDPBody::NEW_ENTITY_ROBOT_GROUND>(
        rt::UDPCommand::NEW_ENTITY_ROBOT_GROUND,
        [this, &spriteManager](const rt::UDPPacket<rt::UDPBody::NEW_ENTITY_ROBOT_GROUND> &packet) {
            handleSharedCreation<rt::UDPBody::NEW_ENTITY_ROBOT_GROUND>(
                "assets/robotGround.json", spriteManager, this->_networkCallbacks, packet
            );
        }
    );
    _udpResponseHandler.registerHandler<rt::UDPBody::NEW_ENTITY_DOBKERATOPS>(
        rt::UDPCommand::NEW_ENTITY_DOBKERATOPS,
        [this, &spriteManager](const rt::UDPPacket<rt::UDPBody::NEW_ENTITY_DOBKERATOPS> &packet) {
            _networkCallbacks.pushBack([packet, &spriteManager](ecs::Registry &reg) {
                const auto &pos = packet.body.pos;
                auto sharedEntityId = packet.sharedEntityId;

                ecs::ClientEntityFactory::createClientEntityFromJSON(
                    reg, spriteManager, "assets/dobkeratops.json", pos.x, pos.y, sharedEntityId
                );
            });
        }
    );

    _udpResponseHandler.registerHandler<rt::UDPBody::NEW_ENTITY_DOBKERATOPS_PART>(
        rt::UDPCommand::NEW_ENTITY_DOBKERATOPS_PART,
        [this, &spriteManager](const rt::UDPPacket<rt::UDPBody::NEW_ENTITY_DOBKERATOPS_PART> &packet) {
            _networkCallbacks.pushBack([packet, &spriteManager](ecs::Registry &reg) {
                const auto &pos = packet.body.pos;
                auto sharedEntityId = packet.sharedEntityId;

                std::string partJson = "assets/dobkeratops_segment.json";
                ecs::ClientEntityFactory::createClientEntityFromJSON(
                    reg, spriteManager, partJson, pos.x, pos.y, sharedEntityId
                );
            });
        }
    );
    _udpResponseHandler.registerHandler<rt::UDPBody::MOVE_ENTITY>(
        rt::UDPCommand::MOVE_ENTITY,
        [this](const rt::UDPPacket<rt::UDPBody::MOVE_ENTITY> &packet) {
            _networkCallbacks.pushBack([packet](ecs::Registry &reg) {
                try {
                    if (reg.hasComponent<ecs::component::SelfPlayer>(reg.getLocalEntity().at(packet.sharedEntityId))) {
                        return;
                    }
                    if (reg.hasComponent<ecs::component::Position>(reg.getLocalEntity().at(packet.sharedEntityId))) {
                        reg.getComponent<ecs::component::Position>(reg.getLocalEntity().at(packet.sharedEntityId))
                            .value() = packet.body.pos;
                    }
                    if (reg.hasComponent<ecs::component::Velocity>(reg.getLocalEntity().at(packet.sharedEntityId))) {
                        reg.getComponent<ecs::component::Velocity>(reg.getLocalEntity().at(packet.sharedEntityId))
                            .value() = packet.body.vel;
                    }
                } catch (const std::exception &e) {
                    eng::logTimeWarning(e.what());
                }
            });
        }
    );
    _udpResponseHandler.registerHandler<rt::UDPBody::DEL_ENTITY>(
        rt::UDPCommand::DEL_ENTITY,
        [this, &spriteManager](const rt::UDPPacket<rt::UDPBody::DEL_ENTITY> &packet) {
            try {
                _networkCallbacks.pushBack([sharedEntityId = packet.sharedEntityId,
                                            &spriteManager](ecs::Registry &reg) {
                    if (reg.hasComponent<ecs::component::ScoreEarned>(reg.getLocalEntity().at(sharedEntityId))) {
                        auto &selfPlayer = reg.getComponents<ecs::component::SelfPlayer>();
                        auto &score = reg.getComponents<ecs::component::Score>();
                        ecs::Zipper<ecs::component::Score, ecs::component::SelfPlayer> zip(score, selfPlayer);
                        for (const auto &[actualScore, self] : zip) {
                            actualScore.value +=
                                reg.getComponent<ecs::component::ScoreEarned>(reg.getLocalEntity().at(sharedEntityId))
                                    .value()
                                    .points;
                        }
                    }
                    auto entity = reg.getLocalEntity().at(sharedEntityId);
                    if (reg.hasComponent<ecs::component::OnDeath>(entity)) {
                        if (reg.hasComponent<ecs::component::Position>(entity)) {
                            int x = reg.getComponent<ecs::component::Position>(entity)->x;
                            int y = reg.getComponent<ecs::component::Position>(entity)->y;
                            ecs::ClientEntityFactory::createClientEntityFromJSON(
                                reg, spriteManager, reg.getComponent<ecs::component::OnDeath>(entity)->entity, x, y
                            );
                        } else {
                            ecs::ClientEntityFactory::createClientEntityFromJSON(
                                reg, spriteManager, reg.getComponent<ecs::component::OnDeath>(entity)->entity
                            );
                        }
                    }
                    reg.killEntity(reg.getLocalEntity().at(sharedEntityId));
                });
            } catch (const std::exception &e) {
                eng::logTimeWarning(e.what());
            }
        }
    );
    _udpResponseHandler.registerHandler<rt::UDPBody::PING>(
        rt::UDPCommand::PING,
        [this](const rt::UDPPacket<rt::UDPBody::PING> &packet) {
            long currentTime = std::chrono::duration_cast<std::chrono::microseconds>(
                                   std::chrono::high_resolution_clock::now().time_since_epoch()
            )
                                   .count();
            _metrics.getMetric(rt::GameMetric::PING).lastComputedMetric = (currentTime - packet.body.sendTime) / 1000.0;
        }
    );
}
