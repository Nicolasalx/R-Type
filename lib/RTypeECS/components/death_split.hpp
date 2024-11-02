/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** death_timer
*/

#pragma once

#include <vector>

namespace ecs::component {

/**
 * @struct DeathSplit
 * @brief Component that manages the splitting behavior of an entity upon death.
 *
 * The `DeathSplit` component defines how an entity splits into multiple parts
 * when it is destroyed. It includes a count of how many splits to perform and
 * a list of offsets for the positions of the split parts.
 */
struct DeathSplit {
    /**
     * @brief Number of splits to perform upon death.
     *
     * This value determines how many parts the entity will be divided into when
     * it is destroyed. The default is set to 2.
     */
    int splitCount = 2;

    /**
     * @brief Offsets for the split parts' positions.
     *
     * A vector of pairs representing the x and y offsets for each part created
     * during the split. These offsets define how far from the original position
     * each split part will be placed.
     */
    std::vector<std::pair<float, float>> offsets;
};

} // namespace ecs::component
