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

namespace ecs::component {

struct ParticleSpawner {
    sf::Vector2f offset;
    std::shared_ptr<particles::TextureParticleSystem> system;
    particles::PointSpawner *spawner = nullptr;
    std::shared_ptr<sf::Texture> texture;
};

} // namespace ecs::component
