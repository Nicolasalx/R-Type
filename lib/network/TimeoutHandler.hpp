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
#include "TickRateManager.hpp"
#include "udp/UDPServer.hpp"

using asio::ip::udp;

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

    void addTimeoutPacket(std::vector<char> &&packet, size_t packetId, UDPServer &udp)
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

    void removeClient(size_t packetId, UDPServer::client_id_t clientId)
    {
        std::lock_guard<std::mutex> lck(_mut);
        for (auto &timeoutPacket : _timeoutPackets) {
            if (packetId == timeoutPacket.packetId) {
                auto itCli = std::find(timeoutPacket.clientIds.begin(), timeoutPacket.clientIds.end(), clientId);
                if (itCli == timeoutPacket.clientIds.end()) {
                    return;
                }
                timeoutPacket.clientIds.erase(itCli);
                if (timeoutPacket.clientIds.empty()) {
                    removeTimeoutPacket(packetId);
                }
                return;
            }
        }
    }

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
            udp.send(cli, reinterpret_cast<const char *>(&packet.packet), packet.packet.size());
        }
    }

    bool _state = true;

    std::thread _tickRateThread;
    std::mutex _mut;

    std::vector<PacketInfos> _timeoutPackets;
    TickRateManager<size_t> _tickRateManager;
};

} // namespace ntw
