# Developer Documentation

## Introduction

This document provides an overview of the project to help new developers get acquainted with its structure and functionality. The focus is on practical information to understand the project broadly, rather than delving into the minutiae of the code. This documentation aims to provide a clear understanding of the project's architecture, main systems, and development guidelines.


## Architectural Overview

This project is built around an Entity-Component-System (ECS) architecture, which separates concerns into entities (game objects), components (data associated with entities), and systems (logic that processes entities with specific components). This provides flexibility and scalability, particularly important for managing the complexity of multiplayer game logic.

## Architectural Diagrams

- **Overview Diagram**:
```mermaid
graph TD
  A[Client Subsystem] --> |Uses| FACTORY
  A --> |Uses| NETWORK
  A --> |Uses| UTILS
  A --> |Uses| ImGui-SFML
  B[Server Subsystem] --> |Uses| FACTORY
  B --> |Uses| NETWORK
  B --> |Uses| UTILS

  FACTORY --> |Depends on| ECS
  FACTORY --> |Depends on| NETWORK
  FACTORY --> |Uses| SFML
  NETWORK --> |Uses| Asio
  UTILS --> |Provides| Logger
  UTILS --> |Provides| ArgParser
  UTILS --> |Provides| TrackedException

  Assets[Shared Assets] --> |Shared by| A
  Assets --> |Shared by| B
  Include[Shared Include Directory] --> |Shared by| A
  Include --> |Shared by| B
```

- **Subsystem Diagrams**:
```mermaid
graph TD
  %% Client Subsystem
  A[Client Subsystem] --> |Registers| ClientComponents
  A --> |Registers| ClientSystems

  %% Server Subsystem
  B[Server Subsystem] --> |Registers| ServerComponents
  B --> |Registers| ServerSystems

  %% Component Registration (Grouped)
  ClientComponents[Client Components]
  ClientComponents --> CoreClientComponents[Core Components]
  ClientComponents --> ControlClientComponents[Control Components]
  ClientComponents --> MovementClientComponents[Movement Components]
  ClientComponents --> VisualClientComponents[Visual Components]

  CoreClientComponents --> Position
  CoreClientComponents --> SharedEntity
  ControlClientComponents --> Controllable
  ControlClientComponents --> Hitbox
  MovementClientComponents --> Velocity
  MovementClientComponents --> ShareMovement
  VisualClientComponents --> Drawable
  VisualClientComponents --> Sprite
  VisualClientComponents --> Parallax
  VisualClientComponents --> Animation
  VisualClientComponents --> Health
  VisualClientComponents --> Player
  VisualClientComponents --> Missile
  VisualClientComponents --> SoundEmitter
  VisualClientComponents --> MusicComponent

  ServerComponents[Server Components]
  ServerComponents --> CoreServerComponents[Core Components]
  ServerComponents --> ControlServerComponents[Control Components]
  ServerComponents --> MovementServerComponents[Movement Components]
  ServerComponents --> VisualServerComponents[Visual Components]

  CoreServerComponents --> Position
  CoreServerComponents --> SharedEntity
  ControlServerComponents --> Controllable
  ControlServerComponents --> Hitbox
  MovementServerComponents --> Velocity
  MovementServerComponents --> ShareMovement
  VisualServerComponents --> Drawable
  VisualServerComponents --> Sprite
  VisualServerComponents --> Parallax
  VisualServerComponents --> Animation
  VisualServerComponents --> Health
  VisualServerComponents --> Player
  VisualServerComponents --> Missile
  VisualServerComponents --> AiActor
  VisualServerComponents --> Tag

  %% System Registration (Grouped)
  ClientSystems[Client Systems]
  ClientSystems --> MovementSystems[Movement Systems]
  ClientSystems --> ControlSystems[Control Systems]
  ClientSystems --> RenderSystems[Render Systems]
  ClientSystems --> NetworkSystems[Network Systems]

  MovementSystems --> ControlMove
  MovementSystems --> PositionSystem
  MovementSystems --> ShareMovement
  ControlSystems --> ControlSpecial
  RenderSystems --> DrawSystem
  RenderSystems --> SpriteSystem
  NetworkSystems --> NetworkCallbacks

  ServerSystems[Server Systems]
  ServerSystems --> AISystems[AI Systems]
  ServerSystems --> MovementSystemsServer[Movement Systems]
  ServerSystems --> HealthSystems[Health Systems]
  ServerSystems --> NetworkSystemsServer[Network Systems]

  AISystems --> AIActing
  MovementSystemsServer --> PositionSystem
  MovementSystemsServer --> CollisionSystem
  HealthSystems --> HealthMobCheck
  NetworkSystemsServer --> SendPackets
  ServerSystems --> WaveSpawning
```
## Deep Dive

Don't hesitate to check the [doxygen documentation](https://manuelr-t.github.io/R-Type/) for more information.

## Tutorials and How-To’s

### Build the engine

#### Dependencies:
To install the required dependencies on **Ubuntu** or any Debian-based system, run the following commands:

```bash
sudo apt update
sudo apt-get install -y \
  flex \
  bison \
  javacc \
  libflac-dev \
  libx11-dev \
  libxext-dev \
  libgl1-mesa-dev \
  libudev-dev \
  libopenal-dev \
  libvorbis-dev \
  libxcursor-dev \
  libxrandr-dev \
  libfreetype6-dev
```

#### Requirement
- C++20 or higher compiler
- CMake for build configuration (3.1 or higher)
- Network connectivity for multiplayer mode

#### Build

- Clone the repo
    ```bash
    git clone git@github.com:ManuelR-T/R-Type.git
    cd R-Type
    ```
- Generate build files using CMake:
    ```bash
    mkdir build
    cd build
    cmake ..
    ```
- Build the project:
    ```bash
    make
    ```
    Alternatively, you can build with:

    ```bash
    cmake --build .
    ```

And from now on you can follow the [Readme tutorial](../README.md#run-the-binaries) on how to use the binaries

### Mod the engine

- How to Add a New Entity

    Define the entity JSON:
    Add a new JSON file in the assets/ directory describing the entity's components, such as position, velocity, sprite, and more.

    Register the entity:
    Use the EntityFactory to register and create this entity in your game. For example:

    ```cpp
    entityFactory.createEntityFromJson("assets/new_entity.json");
    ```

- How to Modify Game Systems

    To modify or create a new system, follow these steps:

    1. Create a new system file in lib/factory/systems/.

    2. Define the system logic by processing entities with specific components. For example, in control_move.cpp:

        ```cpp
        for (auto [entity, position, velocity] : registry.view<Position, Velocity>()) {
            position.x += velocity.vx * delta_time;
            position.y += velocity.vy * delta_time;
        }
        ```

    3. Register the system so that it gets processed.