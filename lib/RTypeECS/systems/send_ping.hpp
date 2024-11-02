/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** send_ping
*/

#pragma once

#include "udp/UDPClient.hpp"

/**
 * @namespace ecs::systems
 * @brief Contains system functions for managing game logic.
 */

/**
 * @brief Sends a ping message through the UDP client.
 *
 * The `sendPing` function utilizes the provided UDP client to
 * send a ping message to the server, which can be used for
 * network latency measurement and maintaining connection.
 *
 * @param udpClient Reference to the UDP client responsible for sending network messages.
 */
namespace ecs::systems {
void sendPing(ntw::UDPClient &udpClient);
} // namespace ecs::systems
