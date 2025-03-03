/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main
*/

#include <SFML/Graphics.hpp>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include "RTypeServer.hpp"
#include "RoomManager.hpp"
#include "TCPResponseHandler.hpp"
#include "tcp/TCPServer.hpp"

int main(int argc, const char *argv[])
{
    rts::RoomManager roomManager;
    int port = 0;
    int argValidity = rts::parseArg(argc, argv, roomManager, port);

    if (argValidity == 84) {
        return 84;
    } else if (argValidity) {
        return 0;
    }

    try {
        ntw::TCPServer tcpServer(port);
        rt::TCPResponseHandler responseHandler;

        rts::registerTcpResponse(roomManager, tcpServer, responseHandler);
        tcpServer.registerCommand([&responseHandler](tcp::socket &sock, char *data, std::size_t size) {
            responseHandler.handleResponse(data, size, {std::ref(sock)});
        });

        tcpServer.run();

        std::string str;
        std::cout << "> ";
        while (std::getline(std::cin, str)) {
            if (str == "quit" || str == "exit") {
                break;
            }
            try {
                rts::commandHandler(str, roomManager, tcpServer);
            } catch (const std::exception &) {
                std::cerr << "Invalid command argument\n";
            }
            std::cout << "> ";
        }
    } catch (const std::exception &e) {
        eng::logError(e.what());
    }
    return 0;
}
