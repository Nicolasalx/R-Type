/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ClientTickRate
*/

#pragma once

#include <cstdint>
#include <unordered_map>

namespace rtc {

enum class TickRate : std::uint8_t {
    ENTITY_MOVEMENT,
    CALL_NETWORK_CALLBACKS
};

const std::unordered_map<rtc::TickRate, float> CLIENT_TICKRATE = {
    {rtc::TickRate::ENTITY_MOVEMENT, 60.0},
    {rtc::TickRate::CALL_NETWORK_CALLBACKS, 60.0}
};

} // namespace rtc
