/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** response_handler
*/

#pragma once

#include <any>
#include <cstring>
#include <functional>
#include <string>
#include <vector>
#include "../lib/utils/Logger.hpp"
#include "../lib/utils/TrackedException.hpp"
#include "RTypeConst.hpp"
#include "RTypeUDPProtol.hpp"
#include <type_traits>
#include <unordered_map>

namespace rt {

class UDPResponseHandler {
    public:
    UDPResponseHandler() = default;

    template <typename T>
    void registerHandler(rt::UDPCommand cmd, std::function<void(const rt::UDPPacket<T> &)> handler)
    {
        _handler[cmd] = [handler](const char *packetData) {
            handler(*reinterpret_cast<const rt::UDPPacket<T> *>(packetData));
        };
    }

    template <typename T>
    void registerHandler(
        rt::UDPCommand cmd,
        std::function<void(const rt::UDPPacket<T> &, const std::vector<std::any> &)> handler
    )
    {
        _specialHandler[cmd] = [handler](const char *packetData, const std::vector<std::any> &arg) {
            handler(*reinterpret_cast<const rt::UDPPacket<T> *>(packetData), arg);
        };
    }

    void handleResponse(const char *data, std::size_t size, const std::vector<std::any> &arg = {})
    {
        const char *ptr = data;
        const rt::UDPPacket<UDPBody::EMPTY> *header = nullptr;

        while (size >= sizeof(rt::UDPPacket<UDPBody::EMPTY>)) {
            header = reinterpret_cast<const rt::UDPPacket<UDPBody::EMPTY> *>(ptr);

            if (header->magic != rt::UDP_MAGIC) {
                eng::logWarning("Invalid UDP magic received !");
                break;
            }
            if (header->size > size) {
                eng::logWarning("Packet of invalid size received !");
                break;
            }
            if (_handler.contains(header->cmd)) {
                _handler.at(header->cmd)(ptr);
            } else if (_specialHandler.contains(header->cmd)) {
                _specialHandler.at(header->cmd)(ptr, arg);
            } else {
                throw eng::TrackedException(
                    "Response without handler: " +
                    std::to_string(static_cast<std::underlying_type_t<rt::UDPCommand>>(header->cmd)) + '.'
                );
            }
            ptr += header->size;
            size -= header->size;
        }
        if (size != 0) {
            eng::logWarning("Fail to analyze the entire UDP packet: " + std::to_string(size) + " byte remaining.");
        }
    }

    private:
    std::unordered_map<rt::UDPCommand, std::function<void(const char *)>> _handler;
    std::unordered_map<rt::UDPCommand, std::function<void(const char *, const std::vector<std::any> &)>>
        _specialHandler;
};

} // namespace rt
