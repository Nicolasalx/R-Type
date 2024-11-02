/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** tick_rate_manager
*/

#pragma once

#include <SFML/Window/Event.hpp>
#include <unordered_map>

namespace ntw {

/**
 * @brief A template class for managing tick rates.
 *
 * The `TickRateManager` class allows for tracking and managing the frequency of updates
 * for various entities identified by a unique ID. It ensures that updates occur at specified
 * intervals, allowing for precise timing control in systems that require it.
 *
 * @tparam T The type representing the unique identifier for each tick rate.
 */
template <typename T>
class TickRateManager {
    public:
    /**
     * @brief Adds a tick rate for a specified ID.
     *
     * This method associates an ID with a tick rate, which defines how frequently updates
     * should occur for that ID.
     *
     * @param id The unique identifier for the tick rate.
     * @param tickRate The rate at which to update, in ticks per second.
     */
    void addTickRate(const T &id, float tickRate)
    {
        _tickRate[id] = {._time_between_tick = (1 / tickRate), ._elapsed_time = 0};
    }

    /**
     * @brief Removes the tick rate associated with a specified ID.
     *
     * @param id The unique identifier of the tick rate to remove.
     */
    void removeTickRate(const T &id)
    {
        _tickRate.erase(id);
    }

    /**
     * @brief Checks if an update is needed for a specified ID based on elapsed time.
     *
     * This method determines whether the time elapsed since the last update exceeds
     * the configured tick rate, allowing for an update to occur.
     *
     * @param id The unique identifier for which to check if an update is needed.
     * @param dt The elapsed time since the last update in seconds.
     * @return True if an update is needed; false otherwise.
     */
    bool needUpdate(const T &id, float dt)
    {
        auto &[time_between_tick, elapsed_time] = _tickRate.at(id);
        elapsed_time += dt;
        if (elapsed_time >= time_between_tick) {
            elapsed_time -= time_between_tick;
            return true;
        }
        return false;
    }

    private:
    struct TickData {
        float _time_between_tick = 0; ///< Time interval between updates for this tick rate.
        float _elapsed_time = 0;      ///< Time elapsed since the last update for this tick rate.
    };

    std::unordered_map<T, TickData> _tickRate; ///< Map of tick rates indexed by unique ID.
};

} // namespace ntw
