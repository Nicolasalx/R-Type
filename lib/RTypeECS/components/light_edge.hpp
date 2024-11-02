/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** light_edge
*/

#pragma once

#include "Candle/LightSource.hpp"

namespace ecs::component {

struct LightEdge {
    candle::EdgeVector edge;
};

} // namespace ecs::component
