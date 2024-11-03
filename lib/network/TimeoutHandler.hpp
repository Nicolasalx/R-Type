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
#include <cstddef>
#include <cstdio>
#include <mutex>
#include <thread>
#include <vector>
#include "TickRateManager.hpp"
#include "udp/UDPServer.hpp"

using asio::ip::udp;

// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init)

namespace ntw {

class TimeoutHandler {
    struct PacketInfos {
        std::vector<char> packet;
        size_t packetId = 0;
        std::vector<UDPServer::client_id_t> clientIds;
    };

    public:
    TimeoutHandler() = default;

    ~TimeoutHandler()
    {
        _state = false;
        if (_tickRateThread.joinable()) {
            _tickRateThread.join();
        }
    }

    void addTimeoutPacket(const std::vector<char> &packet, size_t packetId, UDPServer &udp)
    {
        std::scoped_lock<std::mutex> lck(_mut);
        std::lock_guard<std::recursive_mutex> lckEndpoints(udp.mut());
        _tickRateManager.addTickRate(packetId, 10);
        std::vector<size_t> clients;

        const auto &ends = udp.endpoints();

        clients.reserve(ends.size());
        for (const auto &end : ends) {
            clients.push_back(end.first);
        }
        _timeoutPackets.push_back({.packet = std::move(packet), .packetId = packetId, .clientIds = std::move(clients)});
    }

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

    bool removeClient(size_t packetId, UDPServer::client_id_t clientId)
    {
        // The lock has to be when cailing the function
        for (auto &timeoutPacket : _timeoutPackets) {
            if (packetId == timeoutPacket.packetId) {
                auto itCli = std::find(timeoutPacket.clientIds.begin(), timeoutPacket.clientIds.end(), clientId);
                if (itCli == timeoutPacket.clientIds.end()) {
                    return false;
                }
                timeoutPacket.clientIds.erase(itCli);
                if (timeoutPacket.clientIds.empty()) {
                    _removeTimeoutPacket(packetId);
                }
                return true;
            }
        }
        return false;
    }

    void runTimeoutChecker(float &dt, UDPServer &udp)
    {
        _tickRateThread = std::thread([this, &dt, &udp]() {
            while (_state) {
                std::this_thread::sleep_for(std::chrono::milliseconds(20));

                _checkNewUdpClients(udp);
                _checkTickRates(dt, udp);
            }
        });
    }

    std::mutex &mut()
    {
        return _mut;
    }

    private:
    void _checkTickRates(float &dt, UDPServer &udp)
    {
        std::lock_guard<std::mutex> lck(_mut);

        for (auto &packet : _timeoutPackets) {
            if (_tickRateManager.needUpdate(packet.packetId, dt)) {
                _sendAgainPacket(packet, udp);
            }
        }
    }

    void _sendAgainPacket(PacketInfos &packet, UDPServer &udp)
    {
        for (auto cli : packet.clientIds) {
            udp.send(cli, reinterpret_cast<const char *>(packet.packet.data()), packet.packet.size());
        }
    }

    void _addClientsToPackets(UDPServer::client_id_t clientId)
    {
        for (auto &packet : _timeoutPackets) {
            packet.clientIds.push_back(clientId);
        }
    }

    void _checkNewUdpClients(UDPServer &udp)
    {
        std::lock_guard<std::mutex> lck(_mut);
        std::lock_guard<std::recursive_mutex> lckEndpoints(udp.mut());

        const auto &ends = udp.endpoints();
        for (const auto &end : ends) {
            if (std::find_if(_knownClients.begin(), _knownClients.end(), [&end](auto id) { return id == end.first; }) ==
                _knownClients.end()) {
                _knownClients.push_back(end.first);
                _addClientsToPackets(end.first);
            }
        }
    }

    void _removeTimeoutPacket(size_t packetId)
    {
        auto it = std::find_if(_timeoutPackets.begin(), _timeoutPackets.end(), [packetId](PacketInfos &val) {
            return val.packetId == packetId;
        });
        if (it != _timeoutPackets.end()) {
            _tickRateManager.removeTickRate(packetId);
            _timeoutPackets.erase(it);
        }
    }

    bool _state = true;

    std::thread _tickRateThread;
    std::mutex _mut;

    std::vector<PacketInfos> _timeoutPackets;
    TickRateManager<size_t> _tickRateManager;
    std::vector<UDPServer::client_id_t> _knownClients;
};

} // namespace ntw

// NOLINTEND(cppcoreguidelines-pro-type-member-init)
