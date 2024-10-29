/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ai_actor
*/

#pragma once

#include "entity.hpp"

struct Dobkeratops {
    int attackTicks = 0;
    int neckTicks = 0;
    bool isAttacking = false;
    std::vector<entity_t> neckSegments;
    float targetY = 240.0f;
    int attackPattern = 0;
    int projectilesShot = 0;
    entity_t bossParasite = 0;
    int phase = 0;
};

namespace ecs::component {
struct DobkeratopsState {
    bool initialized = false;
    Dobkeratops state;
};
} // namespace ecs::component
