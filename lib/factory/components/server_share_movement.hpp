/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** share_movement
*/

#pragma once

namespace ecs::component {

/**
 * @struct share_movement
 * @brief Component that send the position and velocity data.
 *
 * The `share_movement` component marks an entity as shared movement to the server.
 */
struct ServerShareMovement {};

} // namespace ecs::component
