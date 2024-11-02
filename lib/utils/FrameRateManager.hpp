/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** FrameRateManager
*/

#pragma once

#include <chrono>

namespace eng {

/**
 * @class FramerateManager
 * @brief Manages frame rate for a game loop.
 *
 * The `FramerateManager` helps in controlling the frame rate by calculating
 * the time needed to maintain a target frames per second (FPS).
 */
class FramerateManager {
    public:
    /**
     * @brief Constructs a FramerateManager with a specified target FPS.
     * @param targetFPS The desired frames per second.
     */
    FramerateManager(double targetFPS);

    /**
     * @brief Sets a new target frame rate.
     * @param targetFPS The new desired frames per second.
     */
    void setFrameRate(double targetFPS);

    /**
     * @brief Starts the frame timing.
     */
    void start();

    /**
     * @brief Ends the frame timing and sleeps if necessary to maintain target FPS.
     */
    void end();

    private:
    std::chrono::duration<double> _targetFrameTime{}; ///< Duration representing the target frame time.
    std::chrono::steady_clock::time_point _startTime; ///< Time point marking the start of the frame.
};

} // namespace eng
