/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ping
*/

#pragma once

namespace ecs::component {

/**
 * @brief Represents the last recorded ping.
 *
 * The ping is a doube that represents the ping in microsecond
 */
struct Ping {
    double ping = 0;
};

} // namespace ecs::component
