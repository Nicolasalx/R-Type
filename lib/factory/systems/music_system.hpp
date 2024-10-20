/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** music_system
*/

#pragma once

#include "../SoundManager.hpp"
#include "Registry.hpp"

namespace ecs::systems {

/**
 * @brief System to manage background music playback.
 *
 * Controls playing, pausing, and stopping music tracks based on MusicComponent.
 *
 * @param reg The ECS registry containing entities and components.
 * @param soundManager Reference to the SoundManager.
 * @param dt Delta time since the last update.
 */
void musicSystem(ecs::Registry &reg, ecs::SoundManager &soundManager, float dt);

} // namespace ecs::systems
