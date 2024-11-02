/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** MetricManager
*/

#pragma once

#include <cstddef>
#include <vector>
#include <unordered_map>

namespace ecs {

/**
 * @brief A class for managing a collection of metrics.
 *
 * The `Metric` class stores a fixed-size history of float values,
 * allowing for the addition of new values and retrieval of the history.
 */
class Metric {
    private:
    std::vector<float> _array; ///< Array to store the history of metric values.
    std::size_t _index = 0;    ///< Current index for adding new values.
    std::size_t _count = 0;    ///< Number of values added to the metric.

    public:
    /**
     * @brief Constructs a Metric with a specified history size.
     *
     * @param historySize The size of the history to maintain.
     */
    Metric(std::size_t historySize) : _array(historySize, 0) {}

    /**
     * @brief Adds a new value to the metric, replacing the oldest value if necessary.
     *
     * @param value The new value to add.
     */
    void addNewValue(float value)
    {
        _array.at(_index) = value;
        _index = (_index + 1) % _array.size();

        if (_count < _array.size()) {
            ++_count;
        }
    }

    /**
     * @brief Retrieves the array of stored metric values.
     *
     * @return const std::vector<float>& Reference to the array of metric values.
     */
    const std::vector<float> &getArray() const
    {
        return _array;
    }

    /**
     * @brief Retrieves the current index for new value addition.
     *
     * @return std::size_t The current index.
     */
    std::size_t getIndex() const
    {
        return _index;
    }

    /**
     * @brief Retrieves the count of values added to the metric.
     *
     * @return std::size_t The count of added values.
     */
    std::size_t getCount() const
    {
        return _count;
    }

    float lastComputedMetric = 0; ///< Stores the last computed metric value.
};

/**
 * @brief A class for managing multiple metrics identified by a key.
 *
 * The `MetricManager` class allows for the addition, removal, and retrieval of
 * metrics using a specified key. This enables organized tracking of various metrics
 * throughout the game.
 *
 * @tparam MetricKey The type representing the key for identifying metrics.
 */
template <typename MetricKey>
class MetricManager {
    private:
    std::unordered_map<MetricKey, ecs::Metric> _metrics; ///< Map of metrics indexed by key.

    public:
    MetricManager() = default; ///< Default constructor.

    /**
     * @brief Constructs a MetricManager with an initial set of metrics.
     *
     * @param metrics A map of metrics to initialize the manager.
     */
    MetricManager(const std::unordered_map<MetricKey, ecs::Metric> &metrics) : _metrics(metrics) {}

    /**
     * @brief Adds a new metric or updates an existing metric in the manager.
     *
     * @param key The key to associate with the new metric.
     * @param newMetric The metric to add.
     */
    void addMetric(const MetricKey &key, ecs::Metric newMetric)
    {
        _metrics[key] = newMetric;
    }

    /**
     * @brief Removes a metric associated with a specified key.
     *
     * @param key The key of the metric to remove.
     */
    void eraseMetric(const MetricKey &key)
    {
        _metrics.erase(key);
    }

    /**
     * @brief Retrieves a metric associated with a specified key.
     *
     * @param key The key of the metric to retrieve.
     * @return const Metric& Reference to the requested metric.
     */
    const Metric &getMetric(const MetricKey &key) const
    {
        return _metrics.at(key);
    }

    /**
     * @brief Retrieves a mutable metric associated with a specified key.
     *
     * @param key The key of the metric to retrieve.
     * @return Metric& Reference to the requested metric.
     */
    Metric &getMetric(const MetricKey &key)
    {
        return _metrics.at(key);
    }
};

} // namespace ecs
