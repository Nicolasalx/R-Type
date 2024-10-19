/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** room_manager
*/

#include "RoomManager.hpp"
#include <cstddef>
#include "RTypeTCPProtol.hpp"

std::size_t rtc::RoomManager::getSelfId() const
{
    return _userId;
}

const std::string &rtc::RoomManager::getCurrentRoom() const
{
    return _currentRoom;
}

std::string &rtc::RoomManager::getCurrentRoom()
{
    return _currentRoom;
}

const std::map<std::size_t, rtc::RoomManager::Player> &rtc::RoomManager::getCurrentRoomPlayer() const
{
    return _rooms.at(_currentRoom).player;
}

const std::map<std::string, rtc::RoomManager::RoomContent> &rtc::RoomManager::getRooms() const
{
    return _rooms;
}

std::map<std::string, rtc::RoomManager::RoomContent> &rtc::RoomManager::getRooms()
{
    return _rooms;
}

const std::string &rtc::RoomManager::getSelfName()
{
    return _userName;
}

std::vector<std::string> &rtc::RoomManager::getChatMsg()
{
    return _chat;
}

const std::vector<std::string> &rtc::RoomManager::getChatMsg() const
{
    return _chat;
}

void rtc::RoomManager::askToCreateRoom(const std::string &roomName, const std::size_t &stage)
{
    rt::TCPPacket<rt::TCPBody::CL_CREATE_ROOM> packet(rt::TCPCommand::CL_CREATE_ROOM);

    packet.body.stage = stage;
    roomName.copy(packet.body.roomName, sizeof(packet.body.roomName) - 1);
    _tcpClient.send(reinterpret_cast<const char *>(&packet), sizeof(packet));
}

void rtc::RoomManager::askToDeleteRoom(const std::string &roomName)
{
    rt::TCPPacket<rt::TCPBody::CL_DELETE_ROOM> packet(rt::TCPCommand::CL_DELETE_ROOM);

    roomName.copy(packet.body.roomName, sizeof(packet.body.roomName) - 1);
    _tcpClient.send(reinterpret_cast<const char *>(&packet), sizeof(packet));
}

void rtc::RoomManager::askToJoinRoom(const std::string &roomName)
{
    rt::TCPPacket<rt::TCPBody::CL_JOIN_ROOM> packet(rt::TCPCommand::CL_JOIN_ROOM);

    packet.body.userId = _userId;
    roomName.copy(packet.body.roomName, sizeof(packet.body.roomName) - 1);
    _userName.copy(packet.body.userName, sizeof(packet.body.userName) - 1);
    _tcpClient.send(reinterpret_cast<const char *>(&packet), sizeof(packet));
}

void rtc::RoomManager::askToLeaveRoom()
{
    rt::TCPPacket<rt::TCPBody::CL_LEAVE_ROOM> packet(rt::TCPCommand::CL_LEAVE_ROOM);

    packet.body.userId = _userId;
    _currentRoom.copy(packet.body.roomName, sizeof(packet.body.roomName) - 1);
    _tcpClient.send(reinterpret_cast<const char *>(&packet), sizeof(packet));
}

void rtc::RoomManager::askToBeReady()
{
    rt::TCPPacket<rt::TCPBody::CL_READY> packet(rt::TCPCommand::CL_READY);

    packet.body.userId = _userId;
    _currentRoom.copy(packet.body.roomName, sizeof(packet.body.roomName) - 1);
    _tcpClient.send(reinterpret_cast<const char *>(&packet), sizeof(packet));
}

void rtc::RoomManager::askToBeNotReady()
{
    rt::TCPPacket<rt::TCPBody::CL_NOT_READY> packet(rt::TCPCommand::CL_NOT_READY);

    packet.body.userId = _userId;
    _currentRoom.copy(packet.body.roomName, sizeof(packet.body.roomName) - 1);
    _tcpClient.send(reinterpret_cast<const char *>(&packet), sizeof(packet));
}

void rtc::RoomManager::udpConnectionReady()
{
    rt::TCPPacket<rt::TCPBody::CL_UDP_CONNECTION_READY> packet(rt::TCPCommand::CL_UDP_CONNECTION_READY);

    packet.body.userId = _userId;
    _currentRoom.copy(packet.body.roomName, sizeof(packet.body.roomName) - 1);
    _tcpClient.send(reinterpret_cast<const char *>(&packet), sizeof(packet));
}

void rtc::RoomManager::askToSendChatMsg(const std::string &msg)
{
    rt::TCPPacket<rt::TCPBody::CL_SEND_CHAT_MSG> packet(rt::TCPCommand::CL_SEND_CHAT_MSG);

    msg.copy(packet.body.msg, sizeof(packet.body.msg) - 1);
    _tcpClient.send(reinterpret_cast<const char *>(&packet), sizeof(packet));
}
