/*
** EPITECH PROJECT, 2024
** network
** File description:
** UDPServer
*/

#include "UDPClient.hpp"
#include <asio.hpp>
#include <asio/placeholders.hpp>
#include "../../utils/Logger.hpp"

ntw::UDPClient::UDPClient(const std::string &host, int port) : _endpoint(asio::ip::make_address(host), port), _sock(_io)
{
    _sock.open(udp::v4());
}

void ntw::UDPClient::_asioRun()
{
    _sock.async_receive_from(asio::buffer(_buff), _endpoint, [this](asio::error_code ec, std::size_t bytes) {
        _handleRecv(std::forward<decltype(ec)>(ec), std::forward<decltype(bytes)>(bytes));
    });
}

void ntw::UDPClient::_handleRecv(asio::error_code ec, std::size_t bytes)
{
    if (ec) {
        eng::logError("Receive error: " + ec.message());
        return;
    }
    _recvHandler(_buff.data(), bytes);
    _asioRun();
}

void ntw::UDPClient::send(const char *data, std::size_t size)
{
    _sock.async_send_to(asio::buffer(data, size), _endpoint, [](asio::error_code ec, std::size_t /* bytes */) {
        if (!ec) {
        } else {
            eng::logError("Send error: " + ec.message());
        }
    });
}

void ntw::UDPClient::run()
{
    _recvThread = std::thread([this]() {
        _asioRun();
        _io.run();
        eng::logInfo("Client terminated");
    });
}
