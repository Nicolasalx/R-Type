/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** score_earned
*/

#pragma once

namespace ecs::component {

/**
 * @brief Represents the score earned when you kill a monster.
 *
 * The score is an integer that represent the score earned by all the player when they kill an enemy.
 */

struct ScoreEarned {
    int points = 0;
};

} // namespace ecs::component
