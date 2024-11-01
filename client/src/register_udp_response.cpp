/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** register_udp_response
*/

#include <atomic>
#include <cstddef>
#include <cstdio>
#include <exception>
#include <string>
#include "ClientEntityFactory.hpp"
#include "GameManager.hpp"
#include "Logger.hpp"
#include "RTypeClient.hpp"
#include "RTypeUDPProtol.hpp"
#include "Registry.hpp"
#include "SpriteManager.hpp"
#include "Zipper.hpp"
#include "components/controllable.hpp"
#include "components/health.hpp"
#include "components/player.hpp"
#include "components/position.hpp"
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
    const std::shared_ptr<ImFont> &font,
    ntw::UDPClient &udpClient
)
{
    networkCallbacks.pushBack([packet, &spriteManager, userId, font, &udpClient](ecs::Registry &reg) {
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
        auto packSer = packet.serialize();
        udpClient.send(reinterpret_cast<char const *>(packSer.data()), packSer.size());
    });
}

template <typename T>
static void handleSharedCreation(
    const std::string &jsonFilePath,
    ecs::SpriteManager &spriteManager,
    eng::SafeList<std::function<void(ecs::Registry &)>> &networkCallbacks,
    const rt::UDPPacket<T> &packet,
    ntw::UDPClient &udpClient
)
{
    auto &pos = packet.body.pos;
    auto sharedEntityId = packet.sharedEntityId;

    networkCallbacks.pushBack([sharedEntityId, pos, &spriteManager, jsonFilePath, &udpClient, packet](ecs::Registry &reg
                              ) {
        ecs::ClientEntityFactory::createClientEntityFromJSON(
            reg, spriteManager, jsonFilePath, pos.x, pos.y, sharedEntityId
        );
        auto packSer = packet.serialize();
        udpClient.send(reinterpret_cast<char const *>(packSer.data()), packSer.size());
    });
}

void rtc::GameManager::_registerUdpResponse(ecs::SpriteManager &spriteManager, ntw::UDPClient &udpClient)
{
    _udpResponseHandler.registerHandler<rt::UDPBody::NEW_ENTITY_STATIC>(
        rt::UDPCommand::NEW_ENTITY_STATIC,
        [this, &spriteManager, &udpClient](const rt::UDPPacket<rt::UDPBody::NEW_ENTITY_STATIC> &packet) {
            handleSharedCreation<rt::UDPBody::NEW_ENTITY_STATIC>(
                "assets/static.json", spriteManager, this->_networkCallbacks, packet, udpClient
            );
        }
    );
    _udpResponseHandler.registerHandler<rt::UDPBody::NEW_ENTITY_PLAYER>(
        rt::UDPCommand::NEW_ENTITY_PLAYER,
        [this, &spriteManager, &udpClient](const rt::UDPPacket<rt::UDPBody::NEW_ENTITY_PLAYER> &packet) {
            handlePlayerCreation(_userId, spriteManager, this->_networkCallbacks, packet, _font, udpClient);
        }
    );
    _udpResponseHandler.registerHandler<rt::UDPBody::NEW_ENTITY_MISSILE>(
        rt::UDPCommand::NEW_ENTITY_MISSILE,
        [this, &spriteManager, &udpClient](const rt::UDPPacket<rt::UDPBody::NEW_ENTITY_MISSILE> &packet) {
            if (packet.body.chargeLevel > 66) {
                handleSharedCreation<rt::UDPBody::NEW_ENTITY_MISSILE>(
                    "assets/beam/beam_large.json", spriteManager, this->_networkCallbacks, packet, udpClient
                );
            } else if (packet.body.chargeLevel > 33 && packet.body.chargeLevel < 66) {
                handleSharedCreation<rt::UDPBody::NEW_ENTITY_MISSILE>(
                    "assets/beam/beam_medium.json", spriteManager, this->_networkCallbacks, packet, udpClient
                );
            } else {
                handleSharedCreation<rt::UDPBody::NEW_ENTITY_MISSILE>(
                    "assets/beam/beam_slim.json", spriteManager, this->_networkCallbacks, packet, udpClient
                );
            }
        }
    );
    _udpResponseHandler.registerHandler<rt::UDPBody::NEW_ENTITY_MISSILE_BALL>(
        rt::UDPCommand::NEW_ENTITY_MISSILE_BALL,
        [this, &spriteManager, &udpClient](const rt::UDPPacket<rt::UDPBody::NEW_ENTITY_MISSILE_BALL> &packet) {
            handleSharedCreation<rt::UDPBody::NEW_ENTITY_MISSILE_BALL>(
                "assets/missileBall.json", spriteManager, this->_networkCallbacks, packet, udpClient
            );
        }
    );
    _udpResponseHandler.registerHandler<rt::UDPBody::NEW_ENTITY_BYDOS_WAVE>(
        rt::UDPCommand::NEW_ENTITY_BYDOS_WAVE,
        [this, &spriteManager, &udpClient](const rt::UDPPacket<rt::UDPBody::NEW_ENTITY_BYDOS_WAVE> &packet) {
            handleSharedCreation<rt::UDPBody::NEW_ENTITY_BYDOS_WAVE>(
                "assets/bydosWave.json", spriteManager, this->_networkCallbacks, packet, udpClient
            );
        }
    );
    _udpResponseHandler.registerHandler<rt::UDPBody::NEW_ENTITY_ROBOT_GROUND>(
        rt::UDPCommand::NEW_ENTITY_ROBOT_GROUND,
        [this, &spriteManager, &udpClient](const rt::UDPPacket<rt::UDPBody::NEW_ENTITY_ROBOT_GROUND> &packet) {
            handleSharedCreation<rt::UDPBody::NEW_ENTITY_ROBOT_GROUND>(
                "assets/robotGround.json", spriteManager, this->_networkCallbacks, packet, udpClient
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

    _udpResponseHandler.registerHandler<rt::UDPBody::NEW_ENTITY_BOSS_PARASITE>(
        rt::UDPCommand::NEW_ENTITY_BOSS_PARASITE,
        [this, &spriteManager](const rt::UDPPacket<rt::UDPBody::NEW_ENTITY_BOSS_PARASITE> &packet) {
            _networkCallbacks.pushBack([packet, &spriteManager](ecs::Registry &reg) {
                const auto &pos = packet.body.pos;
                auto sharedEntityId = packet.sharedEntityId;

                ecs::ClientEntityFactory::createClientEntityFromJSON(
                    reg, spriteManager, "assets/boss_parasite.json", pos.x, pos.y, sharedEntityId
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

    _udpResponseHandler.registerHandler<rt::UDPBody::NEW_ENTITY_BLOB>(
        rt::UDPCommand::NEW_ENTITY_BLOB,
        [this, &spriteManager, &udpClient](const rt::UDPPacket<rt::UDPBody::NEW_ENTITY_BLOB> &packet) {
            handleSharedCreation<rt::UDPBody::NEW_ENTITY_BLOB>(
                "assets/blob.json", spriteManager, this->_networkCallbacks, packet, udpClient
            );
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

    _udpResponseHandler.registerHandler<rt::UDPBody::CHANGE_ANIMATION_STATE>(
        rt::UDPCommand::CHANGE_ANIMATION_STATE,
        [this](const rt::UDPPacket<rt::UDPBody::CHANGE_ANIMATION_STATE> &packet) {
            _networkCallbacks.pushBack([packet](ecs::Registry &reg) {
                try {
                    auto &animation =
                        reg.getComponent<ecs::component::Animation>(reg.getLocalEntity().at(packet.sharedEntityId));
                    if (animation) {
                        animation->state = packet.body.newState;
                    }
                } catch (const std::exception &e) {
                    eng::logTimeWarning(e.what());
                }
            });
        }
    );

    _udpResponseHandler.registerHandler<rt::UDPBody::DEL_ENTITY>(
        rt::UDPCommand::DEL_ENTITY,
        [this, &spriteManager, &udpClient](const rt::UDPPacket<rt::UDPBody::DEL_ENTITY> &packet) {
            _networkCallbacks.pushBack([sharedEntityId = packet.sharedEntityId, packet, &udpClient, &spriteManager](
                                           ecs::Registry &reg
                                       ) {
                try {
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
                        auto onDeath = reg.getComponent<ecs::component::OnDeath>(entity);
                        if (reg.hasComponent<ecs::component::Position>(entity)) {
                            int x = reg.getComponent<ecs::component::Position>(entity)->x + onDeath->x;
                            int y = reg.getComponent<ecs::component::Position>(entity)->y + onDeath->y;
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
                    auto packSer = packet.serialize();
                    udpClient.send(reinterpret_cast<char const *>(packSer.data()), packSer.size());
                } catch (const std::exception &e) {
                    // If entity does not exist, maybe server is late or ahead.
                    eng::logTimeWarning(e.what());
                }
            });
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
    _udpResponseHandler.registerHandler<rt::UDPBody::END_GAME>(
        rt::UDPCommand::END_GAME,
        [&udpClient, this](const rt::UDPPacket<rt::UDPBody::END_GAME> &packet) {
            _gameState.store(GameState::NONE);
            auto packSer = packet.serialize();
            udpClient.send(reinterpret_cast<char const *>(packSer.data()), packSer.size());
        }
    );
    _udpResponseHandler.registerHandler<rt::UDPBody::TAKE_DAMAGE>(
        rt::UDPCommand::TAKE_DAMAGE,
        [this](const rt::UDPPacket<rt::UDPBody::TAKE_DAMAGE> &packet) {
            _networkCallbacks.pushBack([packet](ecs::Registry &reg) {
                try {
                    reg.getComponent<ecs::component::Health>(reg.getLocalEntity().at(packet.sharedEntityId))
                        .value()
                        .currHp -= packet.body.damage;
                } catch (const std::exception &e) {
                    // If entity does not exist, maybe server is late or ahead.
                    eng::logTimeWarning(e.what());
                }
            });
        }
    );
}
