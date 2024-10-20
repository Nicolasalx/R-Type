/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** sound_emitter_system
*/

#pragma once

#include "../SoundManager.hpp"
#include "Registry.hpp"

namespace ecs::systems {

/**
 * @brief System to handle sound effects emitted by entities.
 *
 * Plays sounds based on the SoundEmitter component.
 *
 * @param reg The ECS registry containing entities and components.
 * @param soundManager Reference to the SoundManager.
 * @param dt Delta time since the last update.
 */
void soundEmitterSystem(ecs::Registry &reg, ecs::SoundManager &soundManager);

} // namespace ecs::systems
