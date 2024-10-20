/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** MetricManager
*/

#pragma once

#include <cstddef>
#include <vector>

namespace rtc {

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
} // namespace rtc
