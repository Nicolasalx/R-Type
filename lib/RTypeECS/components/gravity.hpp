/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** gravity
*/

#pragma once

#include <string>

namespace ecs::component {

/**
 * @brief Represents the gravity effect on an entity.
 *
 * This component holds information about the type of enemy affected
 * by gravity, allowing for tailored gravitational interactions
 * based on the specific enemy type.
 */

struct Gravity {
    std::string ennemyType;
};

} // namespace ecs::component
