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

class Metric {
    private:
    std::vector<float> _array;
    std::size_t _index = 0;
    std::size_t _count = 0;

    public:
    Metric(std::size_t historySize) : _array(historySize, 0) {}

    void addNewValue(float value)
    {
        _array.at(_index) = value;
        _index = (_index + 1) % _array.size();

        if (_count < _array.size()) {
            ++_count;
        }
    }

    const std::vector<float> &getArray() const
    {
        return _array;
    }

    std::size_t getIndex() const
    {
        return _index;
    }

    std::size_t getCount() const
    {
        return _count;
    }

    float lastComputedMetric = 0;
};

template <typename MetricKey>
class MetricManager {
    private:
    std::unordered_map<MetricKey, ecs::Metric> _metrics;

    public:
    MetricManager() = default;

    MetricManager(const std::unordered_map<MetricKey, ecs::Metric> &metrics) : _metrics(metrics) {}

    void addMetric(const MetricKey &key, ecs::Metric newMetric)
    {
        _metrics[key] = newMetric;
    }

    void eraseMetric(const MetricKey &key)
    {
        _metrics.erase(key);
    }

    const Metric &getMetric(const MetricKey &key) const
    {
        return _metrics.at(key);
    }

    Metric &getMetric(const MetricKey &key)
    {
        return _metrics.at(key);
    }
};

} // namespace ecs
