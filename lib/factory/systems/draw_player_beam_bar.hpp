/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** draw_player_beam_bar
*/

#pragma once

#include <SFML/Graphics.hpp>
#include "InputManager.hpp"
#include "KeyBind.hpp"
#include "RTypeConst.hpp"
#include "Registry.hpp"

namespace ecs::systems {

void drawPlayerBeamBar(
    Registry &reg,
    const sf::Vector2u &windowSize,
    ecs::InputManager &input,
    sf::Clock &chargeClock,
    const ecs::KeyBind<rt::PlayerAction, sf::Keyboard::Key> &keyBind
);

} // namespace ecs::systems
