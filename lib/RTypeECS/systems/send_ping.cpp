/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** send_ping
*/

#include "send_ping.hpp"
#include "RTypeUDPProtol.hpp"
#include "udp/UDPClient.hpp"

void ecs::systems::sendPing(ntw::UDPClient &udpClient)
{
    rt::UDPPacket<rt::UDPBody::PING> packet(rt::UDPCommand::PING);

    packet.body.sendTime = std::chrono::duration_cast<std::chrono::microseconds>(
                               std::chrono::high_resolution_clock::now().time_since_epoch()
    )
                               .count();

    udpClient.send(reinterpret_cast<const char *>(&packet), packet.size);
}
