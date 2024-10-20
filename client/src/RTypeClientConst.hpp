/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** RTypeClientConst
*/

#pragma once

#include <cstddef>
#include <cstdint>
#include <unordered_map>

namespace rtc {

enum class ClientMetric : std::uint8_t {
    FPS,
    PING
};

const std::unordered_map<rtc::ClientMetric, std::size_t> METRIC_HISTORY_SIZE = {
    {rtc::ClientMetric::FPS, 100.0},
    {rtc::ClientMetric::PING, 100.0}
};

} // namespace rtc
