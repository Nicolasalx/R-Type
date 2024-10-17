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

    NEW_ENTITY_STATIC,
    NEW_ENTITY_PLAYER,
    NEW_ENTITY_MISSILE,
    NEW_ENTITY_MISSILE_BALL,
    NEW_ENTITY_BYDOS_WAVE,
    NEW_ENTITY_ROBOT_GROUND,

    MOVE_ENTITY,

    MOD_ENTITY,
    MOD_ENTITES,
    DEL_ENTITY
};

// NOLINTBEGIN(readability-identifier-naming)
namespace UDPBody {

struct EMPTY {}; // Used in the UDP responce handler

struct MOVE_ENTITY {
    ecs::component::Position pos{};
    ecs::component::Velocity vel{};
};

struct NEW_ENTITY_STATIC {
    MOVE_ENTITY moveData{};
};

struct NEW_ENTITY_PLAYER {
    std::size_t playerId = 0;
    std::size_t playerIndex = 1;
    char playerName[rt::MAX_USER_NAME_SIZE + 1] = {0};
    MOVE_ENTITY moveData{};
};

struct NEW_ENTITY_MISSILE {
    MOVE_ENTITY moveData{};
};

struct NEW_ENTITY_MISSILE_BALL {
    MOVE_ENTITY moveData{};
};

struct NEW_ENTITY_BYDOS_WAVE {
    MOVE_ENTITY moveData{};
};

struct NEW_ENTITY_ROBOT_GROUND {
    MOVE_ENTITY moveData{};
};

struct DEL_ENTITY {};

} // namespace UDPBody

// NOLINTEND(readability-identifier-naming)

/**
 * ! Later on we could have a timestamp member variable
 *   or an id to check late packets
 */
template <typename T>
struct UDPPacket {
    std::size_t magic = rt::UDP_MAGIC;
    std::size_t size = sizeof(*this);
    UDPCommand cmd = UDPCommand::NONE;
    shared_entity_t sharedEntityId = 0;

    T body{};

    std::vector<char> serialize() const
    {
        std::vector<char> buff(this->size);

        std::memcpy(buff.data(), this, size);
        return buff;
    }
};

} // namespace rt
