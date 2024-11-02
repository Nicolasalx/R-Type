/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** TimeoutHandler
*/

#pragma once

#include <algorithm>
#include <asio/ip/udp.hpp>
#include <chrono>
#include <cstdio>
#include <mutex>
#include <thread>
#include <vector>
#include "Logger.hpp"
#include "TickRateManager.hpp"
#include "udp/UDPServer.hpp"

using asio::ip::udp;

namespace ntw {

/**
 * @brief Manages the timeout of packets in a UDP communication context.
 *
 * The `TimeoutHandler` class is responsible for tracking outgoing packets
 * and ensuring that they are resent if a response is not received within
 * a specified time frame. It utilizes a tick rate manager to manage the
 * frequency of these checks.
 */
class TimeoutHandler {
    struct PacketInfos {
        std::vector<char> packet; ///< The data of the packet to be sent.
        size_t packetId = 0; ///< Unique identifier for the packet.
        std::vector<UDPServer::client_id_t> clientIds; ///< List of client IDs to which the packet should be sent.
    };

public:
    TimeoutHandler() = default;

    /**
     * @brief Destructor for the TimeoutHandler.
     *
     * Cleans up the thread used for checking timeouts.
     */
    ~TimeoutHandler();

    /**
     * @brief Adds a packet to be tracked for timeouts.
     *
     * This method stores the packet along with its ID and the client IDs
     * to which it should be sent. The tick rate for the packet is also
     * registered.
     *
     * @param packet The data of the packet to send.
     * @param packetId The unique identifier for the packet.
     * @param udp The UDP server instance used for sending packets.
     */
    void addTimeoutPacket(const std::vector<char> &packet, size_t packetId, UDPServer &udp);

    /**
     * @brief Removes a client ID from all tracked packets.
     *
     * This method removes a specified client ID from the list of clients
     * for all packets. If a client is no longer expected to respond, they
     * can be removed to prevent unnecessary resends.
     *
     * @param clientId The ID of the client to remove.
     */
    void killClientId(UDPServer::client_id_t clientId);

    /**
     * @brief Removes a packet from tracking based on its ID.
     *
     * This method unregisters the packet and its associated tick rate
     * from the handler.
     *
     * @param packetId The unique identifier for the packet to remove.
     */
    void removeTimeoutPacket(size_t packetId);

    /**
     * @brief Removes a specific client from a packet's tracking.
     *
     * This method attempts to remove a client from the list of clients
     * associated with a specific packet. If no clients are left, the packet
     * is also removed from tracking.
     *
     * @param packetId The ID of the packet.
     * @param clientId The ID of the client to remove.
     * @return True if the client was successfully removed; false otherwise.
     */
    bool removeClient(size_t packetId, UDPServer::client_id_t clientId);

    /**
     * @brief Starts the timeout checking thread.
     *
     * This method launches a thread that periodically checks for packet
     * timeouts and resends packets as necessary.
     *
     * @param dt The elapsed time since the last update.
     * @param udp The UDP server instance used for sending packets.
     */
    void runTimeoutChecker(float &dt, UDPServer &udp);

private:
    /**
     * @brief Checks the tick rates of the tracked packets.
     *
     * This method iterates over the stored packets and resends them if
     * their tick rate indicates that they should be resent.
     *
     * @param dt The elapsed time since the last update.
     * @param udp The UDP server instance used for sending packets.
     */
    void _checkTickRates(float &dt, UDPServer &udp);

    /**
     * @brief Resends a packet to its associated clients.
     *
     * This method sends the packet data to all clients listed for that packet.
     *
     * @param packet The packet information containing the data and clients.
     * @param udp The UDP server instance used for sending packets.
     */
    void _sendAgainPacket(PacketInfos &packet, UDPServer &udp);

    bool _state = true; ///< Indicates whether the timeout handler is active.

    std::thread _tickRateThread; ///< The thread that checks for timeouts.
    std::mutex _mut; ///< Mutex for synchronizing access to shared resources.

    std::vector<PacketInfos> _timeoutPackets; ///< List of packets being tracked for timeouts.
    TickRateManager<size_t> _tickRateManager; ///< Manager for handling tick rates of packets.
};

} // namespace ntw

// NOLINTEND(cppcoreguidelines-pro-type-member-init)
