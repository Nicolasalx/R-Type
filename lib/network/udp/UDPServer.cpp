/*
** EPITECH PROJECT, 2024
** network
** File description:
** UDPServer
*/

#include "UDPServer.hpp"
#include <asio/placeholders.hpp>
#include <cstddef>
#include <mutex>
#include <string>
#include <utility>
#include "../utils/Logger.hpp"
#include <initializer_list>

void ntw::UDPServer::_handleRecv(udp::endpoint &endpoint, asio::error_code ec, std::size_t bytes)
{
    if (ec) {
        eng::logError(ec.message());
        return;
    }
    if (bytes) {
        _handler(endpoint, _buff.data(), bytes);
    }

    _asioRun();
}

void ntw::UDPServer::_handleEndpoint(udp::endpoint endpoint)
{
    std::lock_guard<std::recursive_mutex> lck(_mut);

    for (auto &[_, end] : _cliEndpoints) {
        if (endpoint == end) {
            return;
        }
    }
    eng::logInfo("New endpoint: " + endpoint.address().to_string() + ':' + std::to_string(endpoint.port()));
    _cliEndpoints[_nbClients++] = std::move(endpoint);
}

void ntw::UDPServer::removeClient(client_id_t id)
{
    std::lock_guard<std::recursive_mutex> lck(_mut);

    auto it = _cliEndpoints.find(id);
    if (it != _cliEndpoints.end()) {
        _cliEndpoints.erase(it);
    }
}

void ntw::UDPServer::send(udp::endpoint &endpoint, const char *data, std::size_t size)
{
    std::lock_guard<std::recursive_mutex> lck(_mut);

    _sock.async_send_to(asio::buffer(data, size), endpoint, [](asio::error_code ec, std::size_t /* bytes */) {
        if (ec) {
            eng::logError(ec.message());
        }
    });
}

void ntw::UDPServer::send(client_id_t id, const char *data, std::size_t size)
{
    std::lock_guard<std::recursive_mutex> lck(_mut);

    auto it = _cliEndpoints.find(id);
    if (it != _cliEndpoints.end()) {
        send(it->second, data, size);
    }
}

void ntw::UDPServer::sendAll(const char *data, std::size_t size)
{
    std::lock_guard<std::recursive_mutex> lck(_mut);

    for (auto &[_, end] : _cliEndpoints) {
        send(end, data, size);
    }
}

void ntw::UDPServer::sendAllExcept(const char *data, std::size_t size, udp::endpoint &endpoint)
{
    std::lock_guard<std::recursive_mutex> lck(_mut);

    for (auto &[_, end] : _cliEndpoints) {
        if (end == endpoint) {
            continue;
        }
        send(end, data, size);
    }
}

void ntw::UDPServer::sendAllExcept(const char *data, std::size_t size, std::initializer_list<udp::endpoint> endpoints)
{
    std::lock_guard<std::recursive_mutex> lck(_mut);

    for (auto &[_, cli_end] : _cliEndpoints) {
        bool isValidEndpoint = true;
        for (const auto &banned : endpoints) {
            if (banned == cli_end) {
                isValidEndpoint = false;
                break;
            }
        }
        if (isValidEndpoint) {
            send(cli_end, data, size);
        }
    }
}

void ntw::UDPServer::registerCommand(std::function<void(udp::endpoint &, char *, std::size_t)> func)
{
    _handler = std::move(func);
}

void ntw::UDPServer::_asioRun()
{
    _sock.async_receive_from(asio::buffer(_buff), _lastEndpoint, [this](asio::error_code ec, std::size_t bytes) {
        _handleEndpoint(_lastEndpoint);
        _handleRecv(std::ref(_lastEndpoint), std::forward<decltype(ec)>(ec), std::forward<decltype(bytes)>(bytes));
    });
}

void ntw::UDPServer::run()
{
    _recvThread = std::thread([this]() {
        _asioRun();
        _io.run();
    });
}
