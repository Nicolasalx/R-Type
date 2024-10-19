/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** register_tcp_response
*/

#include <string>
#include "GameManager.hpp"
#include "RTypeTCPProtol.hpp"
#include "RoomManager.hpp"

void rtc::GameManager::_registerTcpResponse()
{
    _tcpResponseHandler.registerHandler<rt::TCPBody::SER_ROOM_LIST>(
        rt::TCPCommand::SER_ROOM_LIST,
        [this](const rt::TCPPacket<rt::TCPBody::SER_ROOM_LIST> &packet) {
            _roomManager.getRooms()[packet.body.roomName] = {.player{}, .stage = packet.body.stage};
        }
    );
    _tcpResponseHandler.registerHandler<rt::TCPBody::SER_ROOM_CONTENT>(
        rt::TCPCommand::SER_ROOM_CONTENT,
        [this](const rt::TCPPacket<rt::TCPBody::SER_ROOM_CONTENT> &packet) {
            _roomManager.getRooms().rbegin()->second.player[packet.body.userId] =
                rtc::RoomManager::Player{.name = packet.body.userName, .ready = packet.body.ready};
        }
    );
    _tcpResponseHandler.registerHandler<rt::TCPBody::SER_ROOM_READY>(
        rt::TCPCommand::SER_ROOM_READY,
        [this](const rt::TCPPacket<rt::TCPBody::SER_ROOM_READY> &packet) {
            _inLobby = false;
            _gamePort = packet.body.port;
        }
    );
    _tcpResponseHandler.registerHandler<rt::TCPBody::SER_ROOM_CREATED>(
        rt::TCPCommand::SER_ROOM_CREATED,
        [this](const rt::TCPPacket<rt::TCPBody::SER_ROOM_CREATED> &packet) {
            _roomManager.getRooms()[packet.body.roomName] = {.player{}, .stage = packet.body.stage};
        }
    );
    _tcpResponseHandler.registerHandler<rt::TCPBody::SER_ROOM_DELETED>(
        rt::TCPCommand::SER_ROOM_DELETED,
        [this](const rt::TCPPacket<rt::TCPBody::SER_ROOM_DELETED> &packet) {
            _roomManager.getRooms().erase(packet.body.roomName);
        }
    );
    _tcpResponseHandler.registerHandler<rt::TCPBody::SER_ROOM_JOINED>(
        rt::TCPCommand::SER_ROOM_JOINED,
        [this](const rt::TCPPacket<rt::TCPBody::SER_ROOM_JOINED> &packet) {
            _roomManager.getRooms().at(packet.body.roomName).player[packet.body.userId] =
                rtc::RoomManager::Player{.name = packet.body.userName, .ready = false};
            if (this->_userId == packet.body.userId) {
                _roomManager.getCurrentRoom() = packet.body.roomName;
            }
        }
    );
    _tcpResponseHandler.registerHandler<rt::TCPBody::SER_ROOM_LEAVED>(
        rt::TCPCommand::SER_ROOM_LEAVED,
        [this](const rt::TCPPacket<rt::TCPBody::SER_ROOM_LEAVED> &packet) {
            _roomManager.getRooms().at(packet.body.roomName).player.erase(packet.body.userId);
            if (this->_userId == packet.body.userId) {
                _roomManager.getCurrentRoom().clear();
            }
        }
    );
    _tcpResponseHandler.registerHandler<rt::TCPBody::SER_READY>(
        rt::TCPCommand::SER_READY,
        [this](const rt::TCPPacket<rt::TCPBody::SER_READY> &packet) {
            _roomManager.getRooms().at(packet.body.roomName).player.at(packet.body.userId).ready = true;
        }
    );
    _tcpResponseHandler.registerHandler<rt::TCPBody::SER_NOT_READY>(
        rt::TCPCommand::SER_NOT_READY,
        [this](const rt::TCPPacket<rt::TCPBody::SER_NOT_READY> &packet) {
            _roomManager.getRooms().at(packet.body.roomName).player.at(packet.body.userId).ready = false;
        }
    );
    _tcpResponseHandler.registerHandler<rt::TCPBody::SER_ROOM_IN_GAME>(
        rt::TCPCommand::SER_ROOM_IN_GAME,
        [this](const rt::TCPPacket<rt::TCPBody::SER_ROOM_IN_GAME> &packet) {
            _roomManager.getRooms().at(packet.body.roomName).joinable = false;
        }
    );
    _tcpResponseHandler.registerHandler<rt::TCPBody::SER_ALL_UDP_CONNECTION_READY>(
        rt::TCPCommand::SER_ALL_UDP_CONNECTION_READY,
        [this](const rt::TCPPacket<rt::TCPBody::SER_ALL_UDP_CONNECTION_READY> &) {
            this->_allUDPClientReady.set_value(true);
        }
    );
    _tcpResponseHandler.registerHandler<rt::TCPBody::SER_NEW_CHAT_MSG>(
        rt::TCPCommand::SER_NEW_CHAT_MSG,
        [this](const rt::TCPPacket<rt::TCPBody::SER_NEW_CHAT_MSG> &packet) {
            _roomManager.getChatMsg().emplace_back(packet.body.msg);
        }
    );
}
