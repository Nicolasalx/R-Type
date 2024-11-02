## RType TCP Protocol

### Table of Contents

1. Introduction
2. Message Format
   - 2.1 TCP Packet Structure
3. Commands Overview
   - 3.1 Client-to-Server Commands
   - 3.2 Server-to-Client Commands
4. Command and Data Definitions
   - 4.1 Command: CL_NEW_USER
   - 4.2 Command: CL_CREATE_ROOM / SER_ROOM_CREATED
   - 4.3 Command: CL_DELETE_ROOM / SER_ROOM_DELETED
   - 4.4 Command: CL_JOIN_ROOM / SER_ROOM_JOINED
   - 4.5 Command: CL_LEAVE_ROOM / SER_ROOM_LEAVED
   - 4.6 Command: CL_READY / SER_READY
   - 4.7 Command: CL_NOT_READY / SER_NOT_READY
   - 4.8 Command: CL_ROOM_LIST / SER_ROOM_LIST / SER_ROOM_CONTENT
   - 4.9 Command: SER_ROOM_IN_GAME
   - 4.10 Command: SER_ROOM_READY
   - 4.11 Command: CL_UDP_CONNECTION_READY / SER_ALL_UDP_CONNECTION_READY
5. Error Handling

---

### 1. Introduction

The RType TCP Protocol (RT-TCP) is a custom TCP-based protocol designed to facilitate communication between a server and multiple clients in a multiplayer game system. This protocol is used to manage room creation, user interactions, and game status updates within a game server context. The protocol defines a set of commands exchanged over TCP to coordinate room-based sessions, track user readiness, and manage the transition to game states.

This protocol is independent of any specific platform or programming language, making it suitable for cross-platform multiplayer applications.

### 2. Message Format

#### 2.1 TCP Packet Structure

Each message sent between the server and client is encapsulated in a TCP packet, which follows a consistent structure:

| Field       | Size (bytes) | Description                                         |
|-------------|--------------|-----------------------------------------------------|
| `magic`     | 8            | Magic number identifying RT-TCP protocol (0x5443503e) |
| `size`      | 8            | Size of the entire packet (including the header and data) |
| `cmd`       | 8            | Command identifier (see section 3)                  |
| `data`      | Variable     | Data payload specific to the command                |

- **magic**: A fixed value (0x5443503e), used to verify the packet conforms to the RType TCP protocol.
- **size**: The size of the entire packet, including the command and any associated data.
- **cmd**: The command type, which specifies the purpose of the packet and what data follows.
- **data**: A structured payload associated with the command, containing relevant information (e.g., user ID, room name).

### 3. Commands Overview

Commands in RType TCP are divided into two categories:

1. **Client-to-Server (CL)**: Commands sent by the client to initiate actions.
2. **Server-to-Client (SER)**: Commands sent by the server in response to client actions or to propagate game state updates.

#### 3.1 Client-to-Server Commands

| Command Name            | Command ID       | Description                                 |
|-------------------------|------------------|---------------------------------------------|
| `CL_NEW_USER`            | 1                | Registers a new user                        |
| `CL_CREATE_ROOM`         | 2                | Requests the creation of a new game room    |
| `CL_DELETE_ROOM`         | 4                | Requests deletion of a game room            |
| `CL_JOIN_ROOM`           | 6                | Joins a specific game room                  |
| `CL_LEAVE_ROOM`          | 8                | Leaves a specific game room                 |
| `CL_READY`               | 10               | Marks the user as "ready"                   |
| `CL_NOT_READY`           | 12               | Marks the user as "not ready"               |
| `CL_ROOM_LIST`           | 14               | Requests a list of available rooms          |
| `CL_UDP_CONNECTION_READY`| 18               | Notifies readiness for UDP connection setup |

#### 3.2 Server-to-Client Commands

| Command Name               | Command ID       | Description                                 |
|----------------------------|------------------|---------------------------------------------|
| `SER_ROOM_CREATED`          | 3                | Confirms the creation of a room             |
| `SER_ROOM_DELETED`          | 5                | Confirms the deletion of a room             |
| `SER_ROOM_JOINED`           | 7                | Confirms the user has joined a room         |
| `SER_ROOM_LEAVED`           | 9                | Confirms the user has left a room           |
| `SER_READY`                 | 11               | Confirms the user is ready                  |
| `SER_NOT_READY`             | 13               | Confirms the user is not ready              |
| `SER_ROOM_LIST`             | 15               | Provides a list of rooms                    |
| `SER_ROOM_CONTENT`          | 16               | Provides details of users in a specific room|
| `SER_ROOM_IN_GAME`          | 17               | Notifies that a room has started a game     |
| `SER_ROOM_READY`            | 19               | Notifies that the game is ready to start    |
| `SER_ALL_UDP_CONNECTION_READY`| 20             | Notifies that all users are ready for UDP connections |

### 4. Command and Data Definitions

The following sections define each command and the associated data structure.

#### 4.1 Command: CL_NEW_USER

- **Command ID**: 1
- **Description**: Sent by the client to register a new user.
- **Data**:
  - `user_id`: A unique identifier for the user (generated by the client).

```cpp
struct CL_NEW_USER {
    std::size_t user_id = 0;
};
```

#### 4.2 Command: CL_CREATE_ROOM / SER_ROOM_CREATED

- **Command ID**: 2 (Client), 3 (Server)
- **Description**: The client sends `CL_CREATE_ROOM` to request the creation of a new game room. The server responds with `SER_ROOM_CREATED` upon success.
- **Data**:
  - `room_name`: A string representing the name of the room (max 32 characters).
  - `stage`: The current stage or level of the room (default: 1).

```cpp
struct CL_CREATE_ROOM {
    char room_name[rt::MAX_ROOM_NAME_SIZE + 1] = {0};
    std::size_t stage = 1;
};
```

```cpp
struct SER_ROOM_CREATED {
    char room_name[rt::MAX_ROOM_NAME_SIZE + 1] = {0};
    std::size_t stage = 1;
};
```

#### 4.3 Command: CL_DELETE_ROOM / SER_ROOM_DELETED

- **Command ID**: 4 (Client), 5 (Server)
- **Description**: The client requests to delete a room. The server confirms the deletion.
- **Data**:
  - `room_name`: Name of the room to be deleted.

```cpp
struct CL_DELETE_ROOM {
    char room_name[rt::MAX_ROOM_NAME_SIZE + 1] = {0};
};
```

```cpp
struct SER_ROOM_DELETED {
    char room_name[rt::MAX_ROOM_NAME_SIZE + 1] = {0};
};
```

#### 4.4 Command: CL_JOIN_ROOM / SER_ROOM_JOINED

- **Command ID**: 6 (Client), 7 (Server)
- **Description**: The client requests to join a specific room. The server confirms the user has joined.
- **Data**:
  - `room_name`: Name of the room to join.
  - `user_name`: User's name (max 32 characters).
  - `user_id`: Unique user identifier.

```cpp
struct CL_JOIN_ROOM {
    char room_name[rt::MAX_ROOM_NAME_SIZE + 1] = {0};
    char user_name[rt::MAX_USER_NAME_SIZE + 1] = {0};
    std::size_t user_id = 0;
};
```

```cpp
struct SER_ROOM_JOINED {
    char room_name[rt::MAX_ROOM_NAME_SIZE + 1] = {0};
    char user_name[rt::MAX_USER_NAME_SIZE + 1] = {0};
    std::size_t user_id = 0;
};
```

#### 4.5 Command: CL_LEAVE_ROOM / SER_ROOM_LEAVED

- **Command ID**: 8 (Client), 9 (Server)
- **Description**: The client requests to leave a specific room. The server confirms the user has left.
- **Data**:
  - `room_name`: Name of the room to leave.
  - `user_id`: Unique user identifier.

```cpp
struct CL_LEAVE_ROOM {
    char room_name[rt::MAX_ROOM_NAME_SIZE + 1] = {0};
    std::size_t user_id = 0;
};
```

```cpp
struct SER_ROOM_LEAVED {
    char room_name[rt::MAX_ROOM_NAME_SIZE + 1] = {0};
};
```

#### 4.6 Command: CL_READY / SER_READY

- **Command ID**: 10 (Client), 11 (Server)
- **Description**: The client marks itself as ready. The server confirms the user's readiness.
- **Data**:
  - `room_name`: Name of the room.
  - `user_id`: Unique user identifier.

```cpp
struct CL_READY {
    std::size_t user_id = 0;
};
```

```cpp
struct SER_READY {
    std::size_t user_id = 0;
};
```

#### 4.7 Command: CL_NOT_READY / SER_NOT_READY

- **Command ID**: 12 (Client), 13 (Server)
- **Description**: The client marks itself as not ready. The server confirms the status.
- **Data**:
  - `room_name`: Name of the room.
  - `user_id`: Unique user identifier.

```cpp
struct CL_NOT_READY {
    std::size_t user_id = 0;
};
```

```cpp
struct SER_NOT_READY {
    std::size_t user_id = 0;
};
```

#### 4.8 Command: CL_ROOM_LIST / SER_ROOM_LIST / SER_ROOM_CONTENT

- **Command ID**: 14 (Client), 15 (Server), 16 (Server)
- **Description**: The client requests a list of available rooms. The server responds with a list of rooms and their contents.
- **Data**:
  - `room_name`: Room name.
  - `stage`: Current game stage of the room.
  - `user_name`: Name of a user in the room.
  - `user_id`: Unique user identifier.
  - `ready`: Readiness state of the user.

```cpp
struct CL_ROOM_LIST {};
```

```cpp
struct SER_ROOM_LIST {
    char room_name[rt::MAX_ROOM_NAME_SIZE + 1] = {0};
};
```

```cpp
struct SER_ROOM_CONTENT {
    char user_name[rt::MAX_USER_NAME_SIZE + 1] = {0};
    bool is_ready = false;
};
```

#### 4.9 Command: SER_ROOM_IN_GAME

- **Command ID**: 17
- **Description**: The server notifies that the room has transitioned into an in-game state.
- **Data**:
  - `room_name`: Name of the room.

```cpp
struct SER_ROOM_IN_GAME {
    char room_name[rt::MAX_ROOM_NAME_SIZE + 1] = {0};
};
```

#### 4.10 Command: SER_ROOM_READY

- **Command ID**: 19
- **Description**: The server notifies that the game in the room is ready to start.
- **Data**:
  - `port`: UDP port number for the game.

```cpp
struct SER_ROOM_READY {
    bool is_ready = false;
};
```

#### 4.11 Command: CL_UDP_CONNECTION_READY / SER_ALL_UDP_CONNECTION_READY

- **Command ID**: 18 (Client), 20 (Server)
- **Description**: The client notifies readiness for UDP connections. The server confirms that all users are ready.
- **Data**:
  - `room_name`: Name of the room.
  - `user_id`: Unique user identifier.

```cpp
struct CL_UDP_CONNECTION_READY {
    std::size_t user_id = 0;
};
```

```cpp
struct SER_ALL_UDP_CONNECTION_READY {};
```

### 5. Error Handling

Any malformed packets or invalid commands will ignored by the server.
