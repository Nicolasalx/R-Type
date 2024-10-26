/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** check_game_ending
*/

#include "systems/check_game_ending.hpp"

void ecs::systems::checkGameEnding(
    ecs::Registry &reg,
    const std::function<bool(ecs::Registry &)> &pred,
    const std::function<void(ecs::Registry &)> &callback
)
{
    if (pred(reg)) {
        callback(reg);
    }
}
