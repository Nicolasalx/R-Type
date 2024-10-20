/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** TCPClient
*/

#pragma once

#include "../BuffSize.hpp"
#include "AsioClient.hpp"

#include <asio.hpp>
#include <asio/ip/tcp.hpp>

#include <thread>

using asio::ip::tcp;

namespace ntw {

class TCPClient : public ntw::AsioClient {
    public:
    /**
     * @brief Constructor takes the param host of the server and the param port
     *        where the udp socket will connect.
     * @param host Hostname of the server to connect.
     * @param port Port of the server the clients connect to.
     */
    TCPClient(const std::string &host, int port);

    /**
     * @brief Destructor of the TCPClient Object
     */
    ~TCPClient() override;

    /**
     * @brief Run the `io_context` variable member of this class and
     *        the `asio_run` function that is a recursive loop of asynchronous operations.
     */
    void run() override;

    void send(const char *data, std::size_t size) override;

    void registerHandler(std::function<void(const char *, std::size_t)> handler);

    private:
    /**
     * @brief Recursive loop of asynchronous operations (read, write),
     *        handling the read and write on udp server.
     */
    void _asioRun() override;

    std::thread _thread;
    tcp::socket _socket;
    std::string _host;
    int _port;
    std::array<char, BUFF_SIZE> _buff = {0};
    std::function<void(const char *, std::size_t)> _recvHandler;
};

} // namespace ntw
