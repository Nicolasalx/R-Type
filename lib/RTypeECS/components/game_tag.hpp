/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** game_tag
*/

#pragma once

#include <cstdint>

namespace ecs::component {

/**
 * @brief Enum that represent the value that a `Tag<EntityTag>` can have,
 *        the purpose of a tag is to be able to represent any entity in the game logic.
 */
enum class EntityTag : std::uint8_t {
    NONE,

    ALLY,
    ENEMY
};

} // namespace ecs::component
