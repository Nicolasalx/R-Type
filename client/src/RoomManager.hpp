/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** room_manager
*/

#pragma once

#include <cstddef>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "tcp/TCPClient.hpp"

namespace rtc {

class RoomManager {
    public:
    struct Player {
        std::string name;
        bool ready = false;
    };

    struct RoomContent {
        std::map<std::size_t, Player> player;
        bool joinable = true;
        std::size_t stage = 1;
    };

    private:
    std::map<std::string, RoomContent> _rooms;
    std::string _currentRoom;
    std::shared_ptr<ntw::TCPClient> _tcpClient;
    std::size_t _userId;
    std::string _userName;
    std::vector<std::string> _chat;

    public:
    RoomManager(std::shared_ptr<ntw::TCPClient> tcpClient, std::size_t userId, std::string userName)
        : _tcpClient(std::move(tcpClient)), _userId(userId), _userName(std::move(userName))
    {
    }

    std::size_t getSelfId() const;

    const std::string &getCurrentRoom() const;
    std::string &getCurrentRoom();

    const std::map<std::size_t, Player> &getCurrentRoomPlayer() const;

    const std::map<std::string, RoomContent> &getRooms() const;
    std::map<std::string, RoomContent> &getRooms();

    const std::string &getSelfName();

    std::vector<std::string> &getChatMsg();
    const std::vector<std::string> &getChatMsg() const;

    void askToCreateRoom(const std::string &roomName, const std::size_t &stage);
    void askToDeleteRoom(const std::string &roomName);
    void askToJoinRoom(const std::string &roomName);
    void askToLeaveRoom();
    void askToBeReady();
    void askToBeNotReady();
    void udpConnectionReady();

    void askToSendChatMsg(const std::string &msg);
};

} // namespace rtc
