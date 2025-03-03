/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** RTypeUDPProtol
*/

#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <vector>
#include "../lib/ecs/components/position.hpp"
#include "../lib/ecs/components/velocity.hpp"
#include "RTypeConst.hpp"
#include "../lib/ecs/shared_entity.hpp"

namespace rt {
enum class UDPCommand : std::uint8_t {
    NONE,

    NEW_ENTITY_PLAYER,
    NEW_ENTITY_MISSILE,
    NEW_ENTITY_MISSILE_BALL,
    NEW_ENTITY_BYDOS_WAVE,
    NEW_ENTITY_ROBOT_GROUND,
    NEW_ENTITY_DOBKERATOPS,
    NEW_ENTITY_DOBKERATOPS_PART,
    NEW_ENTITY_BOSS_PARASITE,
    NEW_ENTITY_BLOB,
    NEW_HEALTH_PACK,

    TAKE_DAMAGE,
    INCREASE_HEALTH,

    MOVE_ENTITY,
    CHANGE_ANIMATION_STATE,

    DEL_ENTITY,

    PING,
    END_GAME
};

// NOLINTBEGIN(readability-identifier-naming)
namespace UDPBody {

struct EMPTY {}; // Used in the UDP responce handler

struct NEW_ENTITY_PLAYER {
    std::size_t playerId = 0;
    std::size_t playerIndex = 1;
    char playerName[rt::MAX_USER_NAME_SIZE + 1] = {0};
    ecs::component::Position pos{};
};

struct NEW_ENTITY_MISSILE {
    ecs::component::Position pos{};
    ecs::component::Velocity vel{};
    int chargeLevel = 0;
};

struct NEW_ENTITY_MISSILE_BALL {
    ecs::component::Position pos{};
    ecs::component::Velocity vel{};
};

struct NEW_ENTITY_BYDOS_WAVE {
    ecs::component::Position pos{};
};

struct NEW_ENTITY_BOSS_PARASITE {
    ecs::component::Position pos{};
};

struct NEW_ENTITY_ROBOT_GROUND {
    ecs::component::Position pos{};
    ecs::component::Velocity vel{};
};

struct NEW_ENTITY_DOBKERATOPS {
    ecs::component::Position pos{};
    int stage = 1;
};

struct NEW_ENTITY_DOBKERATOPS_PART {
    ecs::component::Position pos{};
    int partIndex = 0;
};

struct NEW_ENTITY_BLOB {
    ecs::component::Position pos{};
};

struct NEW_HEALTH_PACK {
    ecs::component::Position pos{};
};

struct TAKE_DAMAGE {
    int damage = 1;
};

struct INCREASE_HEALTH {
    int health = 1;
};

struct MOVE_ENTITY {
    ecs::component::Position pos{};
    ecs::component::Velocity vel{};
};

struct CHANGE_ANIMATION_STATE {
    char newState[32] = {0};
};

struct DEL_ENTITY {};

struct PING {
    long sendTime = 0;
};

struct END_GAME {
    bool win = false;
};

} // namespace UDPBody

constexpr const auto generatePacketId = &ecs::generateSharedEntityId;

// NOLINTEND(readability-identifier-naming)

/**
 * ! Later on we could have a timestamp member variable
 *   or an id to check late packets
 */
template <typename T>
struct UDPPacket {
    std::size_t magic = rt::UDP_MAGIC;
    std::size_t size = sizeof(*this);
    std::size_t packetId = generatePacketId();
    UDPCommand cmd = UDPCommand::NONE;
    bool ack = false;
    shared_entity_t sharedEntityId = 0;

    T body{};

    UDPPacket(UDPCommand cmd, shared_entity_t sharedEntityId, const T &body, bool ack = false)
        : cmd(cmd), ack(ack), sharedEntityId(sharedEntityId), body(body)
    {
    }

    UDPPacket(UDPCommand cmd, const T &body, bool ack = false) : cmd(cmd), ack(ack), body(body) {}

    UDPPacket(UDPCommand cmd, bool ack = false) : cmd(cmd), ack(ack) {}

    UDPPacket(UDPCommand cmd, shared_entity_t sharedEntityId, bool ack = false)
        : cmd(cmd), ack(ack), sharedEntityId(sharedEntityId)
    {
    }

    UDPPacket(const std::vector<char> &data)
    {
        std::memcpy(this, data, sizeof(*this));
    }

    UDPPacket(std::vector<char> data, char key)
    {
        for (char &current : data) {
            current ^= key;
        }
        std::memcpy(this, data, sizeof(*this));
    }

    std::vector<char> serialize() const
    {
        std::vector<char> buff(this->size);

        std::memcpy(buff.data(), this, size);
        return buff;
    }

    std::vector<char> encrypt(char key) const
    {
        std::vector<char> buff(this->size);

        std::memcpy(buff.data(), this, size);
        for (char &current : buff) {
            current ^= key;
        }
        return buff;
    }
};

} // namespace rt
