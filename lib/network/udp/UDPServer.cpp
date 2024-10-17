/*
** EPITECH PROJECT, 2024
** network
** File description:
** UDPServer
*/

#include "UDPServer.hpp"
#include <asio/placeholders.hpp>
#include <cstddef>
#include <string>
#include <utility>
#include "../../utils/Logger.hpp"
#include <initializer_list>

void ntw::UDPServer::_handleRecv(udp::endpoint &, asio::error_code ec, std::size_t bytes)
{
    if (ec) {
        eng::logError(ec.message());
        return;
    }
    if (bytes) {
        _handler(_buff.data(), bytes);
    }

    _asioRun();
}

void ntw::UDPServer::_handleEndpoint(udp::endpoint endpoint)
{
    for (auto &[_, end] : _cliEndpoints) {
        if (endpoint == end) {
            return;
        }
    }
    eng::logInfo("New endpoint: " + endpoint.address().to_string() + ':' + std::to_string(endpoint.port()));
    _cliEndpoints[_nbClients++] = std::move(endpoint);
}

void ntw::UDPServer::send(udp::endpoint &to, const char *data, std::size_t size)
{
    _sock.async_send_to(asio::buffer(data, size), to, [this](asio::error_code ec, std::size_t bytes) {
        if (ec) {
            eng::logError(ec.message());
        } else {
        }
    });
}

void ntw::UDPServer::sendAll(const char *data, std::size_t size)
{
    for (auto &[_, end] : _cliEndpoints) {
        send(end, data, size);
    }
}

void ntw::UDPServer::sendAllExcept(const char *data, std::size_t size, udp::endpoint &endpoint)
{
    for (auto &[_, end] : _cliEndpoints) {
        if (end == endpoint) {
            continue;
        }
        send(end, data, size);
    }
}

void ntw::UDPServer::sendAllExcept(const char *data, std::size_t size, std::initializer_list<udp::endpoint> endpoints)
{
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

void ntw::UDPServer::registerCommand(std::function<void(char *, std::size_t)> func)
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
