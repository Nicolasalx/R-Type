/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** death_timer
*/

#pragma once

#include <vector>

namespace ecs::component {

struct DeathSplit {
    int splitCount = 2;
    std::vector<std::pair<float, float>> offsets;
};

} // namespace ecs::component
