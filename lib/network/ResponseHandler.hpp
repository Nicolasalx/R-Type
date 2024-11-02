/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** response_handler
*/

#pragma once

#include <cstdio>
#include <cstring>
#include <functional>
#include <utility>
#include "../utils/TrackedException.hpp"
#include <unordered_map>

namespace ntw {

/**
 * @brief A template class for handling network responses.
 *
 * The `ResponseHandler` class manages command types and their associated handlers,
 * allowing for the registration and processing of responses received over the network.
 *
 * @tparam CommandType The type representing command identifiers.
 * @tparam PacketType The type representing the structure of incoming packets.
 */
template <typename CommandType, typename PacketType>
class ResponseHandler {
    public:
    /**
     * @brief Constructs a ResponseHandler with a command type getter.
     *
     * @param cmdTypeGetter A function that extracts the command type from a packet.
     */
    ResponseHandler(std::function<CommandType(const PacketType &)> cmdTypeGetter)
        : _cmdTypeGetter(std::move(cmdTypeGetter))
    {
    }

    /**
     * @brief Registers a handler for a specific command type.
     *
     * @param cmd The command type to associate with the handler.
     * @param handler A function to handle packets of the associated command type.
     */
    void registerHandler(CommandType cmd, std::function<void(const PacketType &)> handler)
    {
        _handler[cmd] = std::move(handler);
    }

    /**
     * @brief Handles a received response by processing the packet data.
     *
     * @param data Pointer to the raw data of the packet.
     * @param size The size of the received data.
     * @throws eng::TrackedException if the received data size is incorrect or if there is no handler for the command
     * type.
     */
    void handleResponse(const char *data, std::size_t size)
    {
        PacketType msg{};
        CommandType cmdType;

        if (size != sizeof(msg)) {
            throw eng::TrackedException("Recv msg with bad size: " + std::to_string(size) + '.');
        }
        std::memcpy(&msg, data, sizeof(msg));
        cmdType = _cmdTypeGetter(msg);
        if (_handler.contains(cmdType)) {
            _handler[cmdType](msg);
        } else {
            throw eng::TrackedException(
                "Response without handler: " + std::to_string(static_cast<std::size_t>(cmdType)) + '.'
            );
        }
    }

    private:
    std::unordered_map<CommandType, std::function<void(const PacketType &)>>
        _handler;                                                  ///< Map of command types to their handlers.
    std::function<CommandType(const PacketType &)> _cmdTypeGetter; ///< Function to extract command type from a packet.
};

} // namespace ntw
