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
