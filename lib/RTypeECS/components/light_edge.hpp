/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** light_edge
*/

#pragma once

#include "Candle/LightSource.hpp"

/**
 * @struct LightEdge
 * @brief Represents the edge of a light source in the game.
 *
 * The `LightEdge` structure is used to define the boundaries of a light
 * source, allowing for the accurate rendering and behavior of light
 * within the game environment. It utilizes the `EdgeVector` type
 * from the Candle library to manage the light's edges.
 */
namespace ecs::component {

struct LightEdge {
    /**
     * @brief The edges defining the light source.
     *
     * An instance of `candle::EdgeVector` that holds the geometric data
     * for the light's edges, determining how the light interacts with
     * the environment and other entities.
     */
    candle::EdgeVector edge;
};

} // namespace ecs::component
