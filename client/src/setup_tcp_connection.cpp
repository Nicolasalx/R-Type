/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** setup_tcp_connection
*/

#include "GameManager.hpp"
#include "RTypeTCPProtol.hpp"

void rtc::GameManager::_setupTcpConnection()
{
    this->_registerTcpResponse();
    _tcpClient.registerHandler([this](const char *data, std::size_t size) {
        _tcpResponseHandler.handleResponse(data, size);
    });

    _tcpClient.run();

    {
        rt::TCPPacket<rt::TCPBody::CL_NEW_USER> packet(rt::TCPCommand::CL_NEW_USER);
        packet.body.userId = _userId;
        _tcpClient.send(reinterpret_cast<const char *>(&packet), sizeof(packet));
    }
    {
        rt::TCPPacket<rt::TCPBody::CL_ROOM_LIST> packet(rt::TCPCommand::CL_ROOM_LIST);
        packet.body.userId = _userId;
        _tcpClient.send(reinterpret_cast<const char *>(&packet), sizeof(packet));
    }
}
