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

// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init)

namespace ntw {

class TimeoutHandler {
    struct PacketInfos {
        std::vector<char> packet;                      ///< The packet data.
        size_t packetId = 0;                           ///< Unique identifier for the packet.
        std::vector<UDPServer::client_id_t> clientIds; ///< List of client IDs associated with the packet.
    };

    public:
    /// Default constructor.
    TimeoutHandler() = default;

    /// Destructor that ensures the thread is joined before destruction.
    ~TimeoutHandler()
    {
        _state = false;
        if (_tickRateThread.joinable()) {
            _tickRateThread.join();
        }
    }

    /**
     * @brief Adds a timeout packet to the handler.
     *
     * This method stores the packet along with its ID and the associated client IDs.
     *
     * @param packet The packet data to be tracked.
     * @param packetId The unique identifier for the packet.
     * @param udp Reference to the UDPServer for sending packets.
     */
    void addTimeoutPacket(const std::vector<char> &packet, size_t packetId, UDPServer &udp)
    {
        std::lock_guard<std::mutex> lck(_mut);
        std::lock_guard<std::recursive_mutex> lckEndpoints(udp.mut());
        _tickRateManager.addTickRate(packetId, 1);
        std::vector<size_t> clients;

        const auto &ends = udp.endpoints();

        clients.reserve(ends.size());
        for (const auto &end : ends) {
            clients.push_back(end.first);
        }
        _timeoutPackets.push_back({.packet = std::move(packet), .packetId = packetId, .clientIds = std::move(clients)});
    }

    /**
     * @brief Removes a client ID from all timeout packets.
     *
     * This method is used when a client disconnects to ensure
     * that no further packets are sent to that client.
     *
     * @param clientId The client ID to be removed.
     */
    void killClientId(UDPServer::client_id_t clientId)
    {
        std::lock_guard<std::mutex> lck(_mut);
        for (auto &timeoutPacket : _timeoutPackets) {
            auto itCli = std::find(timeoutPacket.clientIds.begin(), timeoutPacket.clientIds.end(), clientId);
            if (itCli == timeoutPacket.clientIds.end()) {
                continue;
            }
            timeoutPacket.clientIds.erase(itCli);
        }
    }

    /**
     * @brief Removes a timeout packet based on its ID.
     *
     * This is used to clean up packets that are no longer needed.
     *
     * @param packetId The ID of the packet to be removed.
     */
    void removeTimeoutPacket(size_t packetId)
    {
        auto it = std::find_if(_timeoutPackets.begin(), _timeoutPackets.end(), [packetId](PacketInfos &val) {
            return val.packetId == packetId;
        });
        if (it != _timeoutPackets.end()) {
            _tickRateManager.removeTickRate(packetId);
            _timeoutPackets.erase(it);
        }
    }

    /**
     * @brief Removes a specific client from a specific packet.
     *
     * If the client is the last one associated with the packet,
     * the packet is also removed.
     *
     * @param packetId The ID of the packet.
     * @param clientId The client ID to be removed.
     * @return true if the client was successfully removed; false otherwise.
     */
    bool removeClient(size_t packetId, UDPServer::client_id_t clientId)
    {
        std::lock_guard<std::mutex> lck(_mut);
        for (auto &timeoutPacket : _timeoutPackets) {
            if (packetId == timeoutPacket.packetId) {
                auto itCli = std::find(timeoutPacket.clientIds.begin(), timeoutPacket.clientIds.end(), clientId);
                if (itCli == timeoutPacket.clientIds.end()) {
                    eng::logInfo(std::format("No such client ({}) in packet ({})", clientId, packetId));
                    return false;
                }
                timeoutPacket.clientIds.erase(itCli);
                if (timeoutPacket.clientIds.empty()) {
                    removeTimeoutPacket(packetId);
                }
                return true;
            }
        }
        return false;
    }

    /**
     * @brief Starts the timeout checker in a separate thread.
     *
     * This method checks periodically for packets that need to be resent.
     *
     * @param dt Delta time since the last update.
     * @param udp Reference to the UDPServer for sending packets.
     */
    void runTimeoutChecker(float &dt, UDPServer &udp)
    {
        _tickRateThread = std::thread([this, &dt, &udp]() {
            while (_state) {
                std::this_thread::sleep_for(std::chrono::seconds(1));

                _checkTickRates(dt, udp);
            }
        });
    }

    private:
    /**
     * @brief Checks if any packets need to be resent based on tick rates.
     *
     * This method is called periodically to determine if any packets
     * should be sent again to clients.
     *
     * @param dt Delta time since the last update.
     * @param udp Reference to the UDPServer for sending packets.
     */
    void _checkTickRates(float &dt, UDPServer &udp)
    {
        std::lock_guard<std::mutex> lck(_mut);

        for (auto &packet : _timeoutPackets) {
            if (_tickRateManager.needUpdate(packet.packetId, dt)) {
                _sendAgainPacket(packet, udp);
            }
        }
    }

    /**
     * @brief Resends a packet to all associated clients.
     *
     * This method is used to send a packet again if no acknowledgment
     * has been received from the clients.
     *
     * @param packet The packet information to be resent.
     * @param udp Reference to the UDPServer for sending packets.
     */
    void _sendAgainPacket(PacketInfos &packet, UDPServer &udp)
    {
        for (auto cli : packet.clientIds) {
            udp.send(cli, reinterpret_cast<const char *>(&packet.packet), packet.packet.size());
        }
    }

    bool _state = true; ///< Indicates whether the timeout handler is active.

    std::thread _tickRateThread; ///< Thread for checking timeouts.
    std::mutex _mut;             ///< Mutex for thread safety.

    std::vector<PacketInfos> _timeoutPackets; ///< List of packets being tracked for timeouts.
    TickRateManager<size_t> _tickRateManager; ///< Manager for tracking tick rates of packets.
};

} // namespace ntw

// NOLINTEND(cppcoreguidelines-pro-type-member-init)
