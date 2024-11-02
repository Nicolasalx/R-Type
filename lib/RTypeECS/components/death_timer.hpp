/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** death_timer
*/

#pragma once

namespace ecs::component {

/**
 * @struct DeathTimer
 * @brief Component that manages the countdown to an entity's death.
 *
 * The `DeathTimer` component is responsible for tracking the time remaining
 * before an entity is considered dead. It allows for timed destruction or
 * other death-related behaviors.
 */
struct DeathTimer {
    /**
     * @brief Time in seconds before the entity dies.
     *
     * This value represents the duration (in seconds) before the entity is
     * destroyed. The default value is set to 1.0 seconds.
     */
    float timeToDeath = 1.0f;
};

} // namespace ecs::component
