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

/**
 * @namespace ecs::systems
 * @brief Contains system functions for managing game logic.
 */

/**
 * @brief Renders the player's beam charge bar on the screen.
 *
 * The `drawPlayerBeamBar` function displays the current charge level
 * of the player's beam weapon. It takes into account player input and
 * updates the visual representation of the beam charge based on the
 * specified window size.
 *
 * @param reg Reference to the game registry that holds all game entities and components.
 * @param windowSize The size of the game window, used for positioning the beam bar.
 * @param input Reference to the input manager that handles player input.
 * @param chargeClock A clock used to track the charge time of the beam.
 * @param keyBind Key bindings for player actions, facilitating customizable controls.
 */
namespace ecs::systems {
void drawPlayerBeamBar(
    Registry &reg,
    const sf::Vector2u &windowSize,
    ecs::InputManager &input,
    sf::Clock &chargeClock,
    const ecs::KeyBind<rt::PlayerAction, sf::Keyboard::Key> &keyBind
);
} // namespace ecs::systems
