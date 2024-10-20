/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** register_ecs
*/

#include <any>
#include <cstddef>
#include <functional>
#include <string>
#include <vector>
#include "RTypeServer.hpp"
#include "RTypeTCPProtol.hpp"
#include "TCPResponseHandler.hpp"

void rts::registerTcpResponse(
    rts::RoomManager &roomManager,
    ntw::TCPServer &tcpServer,
    rt::TCPResponseHandler &responseHandler
)
{
    tcpServer.registerDisconnectionHandler([&roomManager, &tcpServer](std::size_t userId) {
        roomManager.playerDisconnected(userId, tcpServer);
    });
    responseHandler.registerHandler<rt::TCPBody::CL_NEW_USER>(
        rt::TCPCommand::CL_NEW_USER,
        [&tcpServer](const rt::TCPPacket<rt::TCPBody::CL_NEW_USER> &packet, const std::vector<std::any> &arg) {
            tcpServer.addUser(std::any_cast<std::reference_wrapper<tcp::socket>>(arg.at(0)).get(), packet.body.userId);
        }
    );
    responseHandler.registerHandler<rt::TCPBody::CL_CREATE_ROOM>(
        rt::TCPCommand::CL_CREATE_ROOM,
        [&roomManager, &tcpServer](const rt::TCPPacket<rt::TCPBody::CL_CREATE_ROOM> &packet) {
            roomManager.createRoom(packet.body.roomName, packet.body.stage, tcpServer);
        }
    );
    responseHandler.registerHandler<rt::TCPBody::CL_DELETE_ROOM>(
        rt::TCPCommand::CL_DELETE_ROOM,
        [&roomManager, &tcpServer](const rt::TCPPacket<rt::TCPBody::CL_DELETE_ROOM> &packet) {
            roomManager.deleteRoom(packet.body.roomName, tcpServer);
        }
    );
    responseHandler.registerHandler<rt::TCPBody::CL_JOIN_ROOM>(
        rt::TCPCommand::CL_JOIN_ROOM,
        [&roomManager, &tcpServer](const rt::TCPPacket<rt::TCPBody::CL_JOIN_ROOM> &packet) {
            roomManager.joinRoom(packet.body.roomName, packet.body.userId, packet.body.userName, tcpServer);
        }
    );
    responseHandler.registerHandler<rt::TCPBody::CL_LEAVE_ROOM>(
        rt::TCPCommand::CL_LEAVE_ROOM,
        [&roomManager, &tcpServer](const rt::TCPPacket<rt::TCPBody::CL_LEAVE_ROOM> &packet) {
            roomManager.leaveRoom(packet.body.roomName, packet.body.userId, tcpServer);
        }
    );
    responseHandler.registerHandler<rt::TCPBody::CL_READY>(
        rt::TCPCommand::CL_READY,
        [&roomManager, &tcpServer](const rt::TCPPacket<rt::TCPBody::CL_READY> &packet) {
            roomManager.playerReady(packet.body.roomName, packet.body.userId, tcpServer);
        }
    );
    responseHandler.registerHandler<rt::TCPBody::CL_NOT_READY>(
        rt::TCPCommand::CL_NOT_READY,
        [&roomManager, &tcpServer](const rt::TCPPacket<rt::TCPBody::CL_NOT_READY> &packet) {
            roomManager.playerNotReady(packet.body.roomName, packet.body.userId, tcpServer);
        }
    );
    responseHandler.registerHandler<rt::TCPBody::CL_ROOM_LIST>(
        rt::TCPCommand::CL_ROOM_LIST,
        [&roomManager, &tcpServer](const rt::TCPPacket<rt::TCPBody::CL_ROOM_LIST> &packet) {
            roomManager.sendListRoom(packet.body.userId, tcpServer);
        }
    );
    responseHandler.registerHandler<rt::TCPBody::CL_UDP_CONNECTION_READY>(
        rt::TCPCommand::CL_UDP_CONNECTION_READY,
        [&roomManager, &tcpServer](const rt::TCPPacket<rt::TCPBody::CL_UDP_CONNECTION_READY> &packet) {
            roomManager.udpPlayerReady(packet.body.roomName, packet.body.userId, tcpServer);
        }
    );
    responseHandler.registerHandler<rt::TCPBody::CL_SEND_CHAT_MSG>(
        rt::TCPCommand::CL_SEND_CHAT_MSG,
        [&roomManager, &tcpServer](const rt::TCPPacket<rt::TCPBody::CL_SEND_CHAT_MSG> &packet) {
            roomManager.sendNewChatMsg(packet.body.msg, tcpServer);
        }
    );
}
