/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** draw_game_ending
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "RTypeClient.hpp"
#include "Registry.hpp"
#include "imgui.h"

/**
 * @namespace ecs::systems
 * @brief Contains system functions for managing game logic.
 */

/**
 * @brief Renders the end game screen.
 *
 * The `drawGameEnding` function displays the end game screen,
 * showing whether the player won or lost, the player's name,
 * and their final score. This function uses ImGui for UI
 * components and SFML for rendering.
 *
 * @param win A boolean indicating whether the player has won the game.
 * @param window Reference to the SFML render window where the end game screen will be drawn.
 * @param windowSize The size of the game window, used for positioning elements on the screen.
 * @param font A shared pointer to the font used for text rendering in the UI.
 * @param background A sprite representing the background of the end game screen.
 * @param playerName The name of the player to display on the screen.
 * @param score The final score of the player, displayed on the end game screen.
 */
namespace ecs::systems {
void drawGameEnding(
    bool win,
    sf::RenderWindow &window,
    const sf::Vector2u &windowSize,
    const std::shared_ptr<ImFont> &font,
    sf::Sprite &background,
    const std::string &playerName,
    const int &score
);
} // namespace ecs::systems
