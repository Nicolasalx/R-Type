/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** RandomGenerator
*/

#pragma once

namespace eng {

/**
 * @class RandomGenerator
 * @brief Provides random number generation utilities.
 *
 * The `RandomGenerator` class offers static methods to generate random
 * floating-point and integer values within specified ranges.
 */
class RandomGenerator {
    public:
    /**
     * @brief Generates a random floating-point number within a specified range.
     * @param min The minimum value (inclusive).
     * @param max The maximum value (inclusive).
     * @return A random floating-point number between min and max.
     */
    static float generate(float min, float max);

    /**
     * @brief Generates a random integer within a specified range.
     * @param min The minimum value (inclusive).
     * @param max The maximum value (inclusive).
     * @return A random integer between min and max.
     */
    static int generate(int min, int max);
};

} // namespace eng
