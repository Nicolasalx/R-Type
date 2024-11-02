/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** send_ping
*/

#pragma once

#include "udp/UDPClient.hpp"

namespace ecs::systems {

void sendPing(ntw::UDPClient &udpClient);

} // namespace ecs::systems
