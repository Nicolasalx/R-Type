/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** OnDeath
*/

#pragma once

#include <string>

namespace ecs::component {

/**
 * @struct OnDeath
 * @brief Make entity spawn another entity on death.
 *
 */
struct OnDeath {
    std::string entity;
    float x = 0;
    float y = 0;
};

} // namespace ecs::component
