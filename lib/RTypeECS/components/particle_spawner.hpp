/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** particle_spawner
*/

#pragma once

#include <memory>
#include "Particles/ParticleSpawner.h"
#include "Particles/ParticleSystem.h"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/System/Vector2.hpp"

/**
 * @struct ParticleSpawner
 * @brief Component that manages the spawning of particles.
 *
 * The `ParticleSpawner` structure is responsible for controlling the
 * generation of particles within the game. It provides the necessary
 * attributes to define the spawner's behavior, including the position
 * offset, the particle system used, and the texture of the particles.
 */
namespace ecs::component {

struct ParticleSpawner {
    /**
     * @brief Offset from the spawner's position.
     *
     * A vector that defines the offset for the position where particles
     * will be spawned, allowing for more flexible particle effects.
     */
    sf::Vector2f offset;

    /**
     * @brief Shared pointer to the particle system.
     *
     * A shared pointer to an instance of `TextureParticleSystem` that
     * handles the behavior and rendering of the particles.
     */
    std::shared_ptr<particles::TextureParticleSystem> system;

    /**
     * @brief Pointer to the point spawner.
     *
     * A pointer to an instance of `PointSpawner`, which defines how
     * particles are emitted from a specific point.
     */
    particles::PointSpawner *spawner = nullptr;

    /**
     * @brief Shared pointer to the particle texture.
     *
     * A shared pointer to an `sf::Texture` that represents the visual
     * appearance of the particles being spawned.
     */
    std::shared_ptr<sf::Texture> texture;
};

} // namespace ecs::component
