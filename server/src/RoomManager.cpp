/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** room_manager
*/

#include "RoomManager.hpp"
#include <cstddef>
#include <future>
#include <iostream>
#include <memory>
#include "GameRunner.hpp"
#include "RTypeTCPProtol.hpp"

void rts::RoomManager::createRoom(const std::string &name, std::size_t stage, ntw::TCPServer &tcpServer)
{
    rt::TCPPacket<rt::TCPBody::SER_ROOM_CREATED> packet(rt::TCPCommand::SER_ROOM_CREATED);

    _rooms[name] = Room{.player = {}, .stage = stage, .game = {}, .clientReady = {}};

    packet.body.stage = stage;
    name.copy(packet.body.roomName, sizeof(packet.body.roomName) - 1);
    tcpServer.sendToAllUser(reinterpret_cast<const char *>(&packet), sizeof(packet));
}

void rts::RoomManager::deleteRoom(const std::string &name, ntw::TCPServer &tcpServer)
{
    rt::TCPPacket<rt::TCPBody::SER_ROOM_DELETED> packet(rt::TCPCommand::SER_ROOM_DELETED);

    if (_rooms.at(name).player.empty()) {
        _rooms.erase(name);
        name.copy(packet.body.roomName, sizeof(packet.body.roomName) - 1);
        tcpServer.sendToAllUser(reinterpret_cast<const char *>(&packet), sizeof(packet));
    }
}

void rts::RoomManager::joinRoom(
    const std::string &name,
    std::size_t playerId,
    const std::string &playerName,
    ntw::TCPServer &tcpServer
)
{
    rt::TCPPacket<rt::TCPBody::SER_ROOM_JOINED> packet(rt::TCPCommand::SER_ROOM_JOINED);

    _rooms.at(name).player[playerId] = PlayerLobby{.name = playerName, .ready = false};

    packet.body.userId = playerId;
    name.copy(packet.body.roomName, sizeof(packet.body.roomName) - 1);
    playerName.copy(packet.body.userName, sizeof(packet.body.userName) - 1);
    tcpServer.sendToAllUser(reinterpret_cast<const char *>(&packet), sizeof(packet));
}

void rts::RoomManager::leaveRoom(const std::string &name, std::size_t playerId, ntw::TCPServer &tcpServer)
{
    rt::TCPPacket<rt::TCPBody::SER_ROOM_LEAVED> packet(rt::TCPCommand::SER_ROOM_LEAVED);
    bool playerWasReady = _rooms.at(name).player.at(playerId).ready;

    packet.body.userId = playerId;
    name.copy(packet.body.roomName, sizeof(packet.body.roomName) - 1);

    _rooms.at(name).player.erase(playerId);
    tcpServer.sendToAllUser(reinterpret_cast<const char *>(&packet), sizeof(packet));
    if (playerWasReady && _rooms.at(name).player.empty()) {
        _rooms.at(name).game->detach();
        _rooms.at(name).stopGame = true;
        deleteRoom(name, tcpServer);
    }
}

void rts::RoomManager::playerReady(
    const std::string &roomName,
    std::size_t playerId,
    int missileSpawnRate,
    int playerMissileSpawnRate,
    ntw::TCPServer &tcpServer
)
{
    {
        rt::TCPPacket<rt::TCPBody::SER_READY> packet(rt::TCPCommand::SER_READY);

        packet.body.userId = playerId;
        packet.body.missileSpawnRate = missileSpawnRate;
        packet.body.playerMissileSpawnRate = playerMissileSpawnRate;
        roomName.copy(packet.body.roomName, sizeof(packet.body.roomName) - 1);

        _rooms.at(roomName).player.at(playerId).ready = true;
        if (missileSpawnRate != 100) {
            _rooms.at(roomName).missileSpawnRate = missileSpawnRate;
        }
        tcpServer.sendToAllUser(reinterpret_cast<const char *>(&packet), sizeof(packet));
    }
    // * check if all player in the room are ready
    bool allReady = true;

    for (const auto &[id, player] : _rooms.at(roomName).player) {
        if (!player.ready) {
            allReady = false;
        }
    }
    if (!allReady) {
        return;
    }

    std::promise<bool> serverReady;
    std::future<bool> server = serverReady.get_future();
    std::future<bool> udpClient = _rooms.at(roomName).clientReady.get_future();
    _rooms.at(roomName).gameRunner = std::make_shared<rts::GameRunner>(
        _nextPort, _rooms.at(roomName).stage, _rooms.at(roomName).missileSpawnRate, this->_debugMode
    );

    _rooms.at(roomName).game = std::make_unique<std::thread>(
        [gameRunner = _rooms.at(roomName).gameRunner](
            bool &stopGame, std::promise<bool> serverReady, std::future<bool> udpClient
        ) {
            serverReady.set_value(true);
            udpClient.wait();
            gameRunner->addWindow(sf::VideoMode(720, 480), "R-Type");
            gameRunner->runGame(stopGame);
        },
        std::ref(_rooms.at(roomName).stopGame),
        std::move(serverReady),
        std::move(udpClient)
    );
    server.wait();

    {
        rt::TCPPacket<rt::TCPBody::SER_ROOM_READY> packet(rt::TCPCommand::SER_ROOM_READY);
        packet.body.port = this->_nextPort++;
        for (const auto &[id, player] : _rooms.at(roomName).player) {
            tcpServer.sendToUser(id, reinterpret_cast<const char *>(&packet), sizeof(packet));
        }
    }
    {
        rt::TCPPacket<rt::TCPBody::SER_ROOM_IN_GAME> packet(rt::TCPCommand::SER_ROOM_IN_GAME);
        roomName.copy(packet.body.roomName, sizeof(packet.body.roomName) - 1);
        tcpServer.sendToAllUser(reinterpret_cast<const char *>(&packet), sizeof(packet));
    }
}

void rts::RoomManager::playerNotReady(const std::string &roomName, std::size_t playerId, ntw::TCPServer &tcpServer)
{
    rt::TCPPacket<rt::TCPBody::SER_NOT_READY> packet(rt::TCPCommand::SER_NOT_READY);

    packet.body.userId = playerId;
    roomName.copy(packet.body.roomName, sizeof(packet.body.roomName) - 1);

    _rooms.at(roomName).player.at(playerId).ready = false;
    tcpServer.sendToAllUser(reinterpret_cast<const char *>(&packet), sizeof(packet));
}

void rts::RoomManager::sendListRoom(std::size_t playerId, ntw::TCPServer &tcpServer)
{
    for (const auto &[name, content] : _rooms) {
        {
            rt::TCPPacket<rt::TCPBody::SER_ROOM_LIST> packet(rt::TCPCommand::SER_ROOM_LIST);

            packet.body.stage = content.stage;
            name.copy(packet.body.roomName, sizeof(packet.body.roomName) - 1);
            tcpServer.sendToUser(playerId, reinterpret_cast<const char *>(&packet), sizeof(packet));
        }
        bool isFullReady = !content.player.empty();
        for (const auto &[id, player] : content.player) {
            {
                rt::TCPPacket<rt::TCPBody::SER_ROOM_CONTENT> packet(rt::TCPCommand::SER_ROOM_CONTENT);
                packet.body.ready = player.ready;
                packet.body.userId = id;
                player.name.copy(packet.body.userName, sizeof(packet.body.userName) - 1);
                tcpServer.sendToUser(playerId, reinterpret_cast<const char *>(&packet), sizeof(packet));
            }
            if (!player.ready) {
                isFullReady = false;
            }
        }
        if (isFullReady) {
            rt::TCPPacket<rt::TCPBody::SER_ROOM_IN_GAME> packet(rt::TCPCommand::SER_ROOM_IN_GAME);
            name.copy(packet.body.roomName, sizeof(packet.body.roomName) - 1);
            tcpServer.sendToUser(playerId, reinterpret_cast<const char *>(&packet), sizeof(packet));
        }
    }
}

void rts::RoomManager::playerDisconnected(std::size_t playerId, ntw::TCPServer &tcpServer)
{
    for (const auto &[roomName, room] : this->_rooms) {
        for (const auto &[id, _] : room.player) {
            if (playerId == id) {
                if (room.gameRunner) {
                    room.gameRunner->killPlayer(playerId);
                }
                this->leaveRoom(roomName, id, tcpServer);
                return;
            }
        }
    }
}

void rts::RoomManager::udpPlayerReady(const std::string &roomName, std::size_t playerId, ntw::TCPServer &tcpServer)
{
    rt::TCPPacket<rt::TCPBody::SER_ALL_UDP_CONNECTION_READY> packet(rt::TCPCommand::SER_ALL_UDP_CONNECTION_READY);
    bool allPlayerReady = true;

    _rooms.at(roomName).player.at(playerId).udpReady = true;
    for (const auto &[_, player] : _rooms.at(roomName).player) {
        if (!player.udpReady) {
            allPlayerReady = false;
        }
    }
    if (allPlayerReady) {
        for (const auto &[id, _] : _rooms.at(roomName).player) {
            tcpServer.sendToUser(id, reinterpret_cast<const char *>(&packet), sizeof(packet));
        }
        _rooms.at(roomName).clientReady.set_value(true);
    }
}

void rts::RoomManager::enableDebugMode()
{
    this->_debugMode = true;
}

void rts::RoomManager::sendNewChatMsg(const std::string &msg, ntw::TCPServer &tcpServer)
{
    rt::TCPPacket<rt::TCPBody::SER_NEW_CHAT_MSG> packet(rt::TCPCommand::SER_NEW_CHAT_MSG);
    msg.copy(packet.body.msg, sizeof(packet.body.msg) - 1);
    tcpServer.sendToAllUser(reinterpret_cast<const char *>(&packet), sizeof(packet));
}

void rts::RoomManager::printRoomList()
{
    std::cout << "┌───────\n";
    for (const auto &[roomName, room] : this->_rooms) {
        std::cout << "│ " << roomName << ", " << (room.gameRunner ? "in game" : "in lobby") << ":\n";
        for (const auto &[id, player] : room.player) {
            std::cout << "│     [" << id << "]: " << player.name << ", " << (player.ready ? "ready" : "not ready")
                      << "\n";
        }
    }
    std::cout << "└───────\n";
}

void rts::RoomManager::banPlayer(const std::string &roomName, std::size_t playerId, ntw::TCPServer &tcpServer)
{
    if (this->_rooms.at(roomName).gameRunner) {
        this->_rooms.at(roomName).gameRunner->killPlayer(playerId);
    }
    this->leaveRoom(roomName, playerId, tcpServer);
}
