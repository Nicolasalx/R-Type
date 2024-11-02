/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ai_actor
*/

#pragma once

#include "entity.hpp"

/**
 * @struct Dobkeratops
 * @brief Represents the state and behavior of the Dobkeratops boss.
 *
 * The `Dobkeratops` structure contains the attributes and state information
 * necessary for managing the Dobkeratops boss, including its attack patterns,
 * neck segments, and overall behavior during the game.
 */
struct Dobkeratops {
    /**
     * @brief Counter for attack timing.
     *
     * This value tracks the number of ticks since the last attack. It is
     * incremented each frame and is used to determine when the boss can
     * attack again.
     */
    int attackTicks = 0;

    /**
     * @brief Counter for neck movement timing.
     *
     * This value tracks the number of ticks for the neck's movement
     * animations or behaviors.
     */
    int neckTicks = 0;

    /**
     * @brief Indicates if the boss is currently attacking.
     *
     * A boolean flag that is set to true when the boss is in the process
     * of attacking and false otherwise.
     */
    bool isAttacking = false;

    /**
     * @brief Segments of the Dobkeratops's neck.
     *
     * A vector containing the entities that represent the segments of the
     * Dobkeratops's neck. This allows for dynamic positioning and behavior
     * of each segment during gameplay.
     */
    std::vector<entity_t> neckSegments;

    /**
     * @brief Target Y position for the boss.
     *
     * The Y coordinate that the boss aims to reach or maintain, usually
     * set to a predefined value like 240.0f for game design purposes.
     */
    float targetY = 240.0f;

    /**
     * @brief Current attack pattern of the boss.
     *
     * An integer representing the current attack pattern being used by
     * the Dobkeratops. Different patterns can change the behavior of the
     * boss during encounters.
     */
    int attackPattern = 0;

    /**
     * @brief Count of projectiles shot by the boss.
     *
     * This value tracks how many projectiles the boss has fired during
     * its current phase or state.
     */
    int projectilesShot = 0;

    /**
     * @brief Entity ID of the boss parasite.
     *
     * An entity_t representing the specific boss parasite associated
     * with the Dobkeratops, used for managing its interactions and
     * state within the game.
     */
    entity_t bossParasite = 0;

    /**
     * @brief Current phase of the boss.
     *
     * An integer that represents the current phase or state of the
     * Dobkeratops, which may affect its behavior and attack patterns.
     */
    int phase = 0;
};

namespace ecs::component {

/**
 * @struct DobkeratopsState
 * @brief Represents the initialization state of the Dobkeratops boss.
 *
 * The `DobkeratopsState` structure holds the initialization flag and
 * the current state of the Dobkeratops, allowing for easy management
 * of its lifecycle within the game.
 */
struct DobkeratopsState {
    /**
     * @brief Indicates whether the Dobkeratops has been initialized.
     *
     * A boolean flag that is set to true once the Dobkeratops has been
     * properly initialized and is ready for gameplay.
     */
    bool initialized = false;

    /**
     * @brief Current state of the Dobkeratops boss.
     *
     * An instance of the `Dobkeratops` structure that holds the current
     * attributes and state information for the boss.
     */
    Dobkeratops state;
};

} // namespace ecs::component
