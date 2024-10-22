/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** RTypeTCPProtol
*/

#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <vector>
#include "RTypeConst.hpp"

namespace rt {

enum class TCPCommand : std::uint8_t {
    NONE,

    CL_NEW_USER,

    CL_CREATE_ROOM,
    SER_ROOM_CREATED,

    CL_DELETE_ROOM,
    SER_ROOM_DELETED,

    CL_JOIN_ROOM,
    SER_ROOM_JOINED,

    CL_LEAVE_ROOM,
    SER_ROOM_LEAVED,

    CL_READY,
    SER_READY,

    CL_NOT_READY,
    SER_NOT_READY,

    CL_ROOM_LIST,     // ask for room list
    SER_ROOM_LIST,    // send room list (one by one)
    SER_ROOM_CONTENT, // send room content (one by one)

    SER_ROOM_IN_GAME,
    SER_ROOM_READY,

    CL_UDP_CONNECTION_READY,
    SER_ALL_UDP_CONNECTION_READY,

    CL_SEND_CHAT_MSG,
    SER_NEW_CHAT_MSG
};

// NOLINTBEGIN(readability-identifier-naming)
namespace TCPBody {

struct EMPTY {}; // Used in the TCP responce handler

struct CL_NEW_USER {
    std::size_t userId = 0;
};

struct CL_CREATE_ROOM {
    char roomName[rt::MAX_ROOM_NAME_SIZE + 1] = {0};
    std::size_t stage = 1;
};

struct SER_ROOM_CREATED {
    char roomName[rt::MAX_ROOM_NAME_SIZE + 1] = {0};
    std::size_t stage = 1;
};

struct CL_DELETE_ROOM {
    char roomName[rt::MAX_ROOM_NAME_SIZE + 1] = {0};
};

struct SER_ROOM_DELETED {
    char roomName[rt::MAX_ROOM_NAME_SIZE + 1] = {0};
};

struct CL_JOIN_ROOM {
    char roomName[rt::MAX_ROOM_NAME_SIZE + 1] = {0};
    char userName[rt::MAX_USER_NAME_SIZE + 1] = {0};
    std::size_t userId = 0;
};

struct SER_ROOM_JOINED {
    char roomName[rt::MAX_ROOM_NAME_SIZE + 1] = {0};
    char userName[rt::MAX_USER_NAME_SIZE + 1] = {0};
    std::size_t userId = 0;
};

struct CL_LEAVE_ROOM {
    char roomName[rt::MAX_ROOM_NAME_SIZE + 1] = {0};
    std::size_t userId = 0;
};

struct SER_ROOM_LEAVED {
    char roomName[rt::MAX_ROOM_NAME_SIZE + 1] = {0};
    std::size_t userId = 0;
};

struct CL_READY {
    char roomName[rt::MAX_ROOM_NAME_SIZE + 1] = {0};
    std::size_t userId = 0;
};

struct SER_READY {
    char roomName[rt::MAX_ROOM_NAME_SIZE + 1] = {0};
    std::size_t userId = 0;
};

struct CL_NOT_READY {
    char roomName[rt::MAX_ROOM_NAME_SIZE + 1] = {0};
    std::size_t userId = 0;
};

struct SER_NOT_READY {
    char roomName[rt::MAX_ROOM_NAME_SIZE + 1] = {0};
    std::size_t userId = 0;
};

struct CL_ROOM_LIST {
    std::size_t userId = 0;
};

struct SER_ROOM_LIST {
    char roomName[rt::MAX_ROOM_NAME_SIZE + 1] = {0};
    std::size_t stage = 1;
};

struct SER_ROOM_CONTENT {
    char userName[rt::MAX_USER_NAME_SIZE + 1] = {0};
    std::size_t userId = 0;
    bool ready = false;
};

struct SER_ROOM_IN_GAME {
    char roomName[rt::MAX_ROOM_NAME_SIZE + 1] = {0};
};

struct SER_ROOM_READY {
    int port = 0;
};

struct CL_UDP_CONNECTION_READY {
    char roomName[rt::MAX_ROOM_NAME_SIZE + 1] = {0};
    std::size_t userId = 0;
};

struct SER_ALL_UDP_CONNECTION_READY {};

struct CL_SEND_CHAT_MSG {
    char msg[MAX_CHAT_MSG_SIZE + 1] = {0};
};

struct SER_NEW_CHAT_MSG {
    char msg[MAX_CHAT_MSG_SIZE + 1] = {0};
};

} // namespace TCPBody

// NOLINTEND(readability-identifier-naming)

template <typename T>
struct TCPPacket {
    std::size_t magic = rt::TCP_MAGIC;
    std::size_t size = sizeof(*this);
    TCPCommand cmd = TCPCommand::NONE;

    T body{};

    TCPPacket(TCPCommand cmd) : cmd(cmd) {}

    TCPPacket(TCPCommand cmd, const T &body) : cmd(cmd), body(body) {}

    TCPPacket(std::vector<char> data, char key)
    {
        for (char &current : data) {
            current ^= key;
        }
        std::memcpy(this, data, sizeof(*this));
    }

    std::vector<char> serialize() const
    {
        std::vector<char> buff(this->size);

        std::memcpy(buff.data(), this, size);
        return buff;
    }

    std::vector<char> encrypt(char key) const
    {
        std::vector<char> buff(this->size);

        std::memcpy(buff.data(), this, size);
        for (char &current : buff) {
            current ^= key;
        }
        return buff;
    }
};
} // namespace rt
