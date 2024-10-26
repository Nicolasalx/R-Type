/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** check_game_ending
*/

#pragma once

#include <functional>
#include "Registry.hpp"

namespace ecs::systems {
void checkGameEnding(
    ecs::Registry &reg,
    const std::function<bool(ecs::Registry &)> &pred,
    const std::function<void(ecs::Registry &)> &callback
);
}
