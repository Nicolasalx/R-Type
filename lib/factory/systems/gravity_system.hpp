/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** gravity_system
*/

#pragma once

#include "Registry.hpp"
#include <SFML/Graphics.hpp>
#include "udp/UDPClient.hpp"

namespace ecs::systems {

/**
 * @brief Apply gravity to ground-based enemies, affecting their movement and behavior
 * based on their `ai_actor` component and associated callback `act`.
 *
 * This system ensures that terrestrial enemies are influenced by gravitational forces,
 * allowing for more realistic interactions within the game environment.
 *
 */
void gravitySystem(Registry &reg, const sf::Vector2u &windowSize, ntw::UDPClient &udpClient);

} // namespace ecs::systems
