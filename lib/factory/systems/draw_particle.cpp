/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** particle_spawner
*/

#include "systems/draw_particle.hpp"
#include "SFML/System/Vector2.hpp"
#include "Zipper.hpp"
#include "components/position.hpp"
#include "components/particle_spawner.hpp"

void ecs::systems::drawParticle(ecs::Registry &reg, const sf::Time &dt, sf::RenderWindow &window)
{
    auto &position = reg.getComponents<ecs::component::Position>();
    auto &particle = reg.getComponents<ecs::component::ParticleSpawner>();

    ecs::Zipper<ecs::component::Position, ecs::component::ParticleSpawner> zip(position, particle);

    for (const auto &[pos, part] : zip) {
        // part.spawner->center = sf::Vector2f(pos.x + part.offset.x, pos.y + part.offset.y);
        part.system->setPosition(sf::Vector2f(pos.x + part.offset.x, pos.y + part.offset.y));
        part.system->update(dt);
        part.system->render(window);
    }
}
