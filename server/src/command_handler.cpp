/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** command_handler
*/

#include <iostream>
#include <string>
#include "RTypeServer.hpp"
#include "tcp/TCPServer.hpp"

void rts::commandHandler(std::string &str, rts::RoomManager &roomManager, ntw::TCPServer &tcpServer)
{
    if (str == "help") {
        std::cout << "    list: list all room and player\n"
            << "    ban: ban a player from his game an kick it from his room\n"
            << "    kick: ban a player from his game an kick it from his room\n"
            << "    broadcast: send a warning message to all player\n"
            << "    create room: create a room\n"
            << "    delete room: delete a room\n"
            << "    debug mode: activate the debug mode\n";
    }
    if (str == "list") {
        roomManager.printRoomList();
    }
    if (str == "ban" || str == "kick") {
        try {
            std::size_t playerId = 0;
            std::cout << "playerId: ";
            std::getline(std::cin, str);
            playerId = std::stoul(str);
            std::cout << "roomName: ";
            std::getline(std::cin, str);
            roomManager.banPlayer(str, playerId, tcpServer);
        } catch (const std::exception &) {
            std::cerr << "Invalid command argument\n";
        }
    }
    if (str == "broadcast") {
        std::cout << "msg: ";
        std::getline(std::cin, str);
        roomManager.sendNewChatMsg("/!\\ SERVER ADMIN:" + str, tcpServer);
    }
    if (str == "create room") {
        std::cout << "room name: ";
        std::getline(std::cin, str);
        roomManager.createRoom(str, 1, tcpServer);
    }
    if (str == "delete room") {
        std::cout << "room name: ";
        std::getline(std::cin, str);
        roomManager.deleteRoom(str, tcpServer);
    }
    if (str == "debug mode") {
        roomManager.enableDebugMode();
        std::cout << "debug mode enable\n";
    }
}
