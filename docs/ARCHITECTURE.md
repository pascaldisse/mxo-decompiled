# Matrix Online Architecture Documentation

## Overview

The Matrix Online is built on a custom game engine derived from the LithTech Jupiter engine. This document outlines the major architectural components and their relationships.

## Engine Architecture

The engine follows a modular, component-based design with clear separation of concerns:

```
Core Engine Layer
├── Memory Management
├── Resource Management
├── Class Management
├── Event System
└── Threading

Game Systems Layer
├── Rendering System
├── Audio System
├── Physics System
├── Animation System
├── Input System
├── Network System
└── UI System

Game Logic Layer
├── Game Objects
├── AI System
├── Mission System
├── Combat System
├── Chat System
└── Character System
```

## Core Classes

### Base Classes

- **CLTBaseClass**: Root class for all engine objects, providing RTTI, reference counting, and basic lifecycle management.
- **CLTObject**: Base class for all interactive objects, adding property support and message handling.
- **CLTGameObject**: Base class for game world entities, adding position, rotation, and collision.
- **CLTSubsystem**: Base class for engine subsystems like rendering, physics, etc.
- **CLTManager**: Base class for singleton managers that coordinate subsystems.

### Key Subsystems

- **CLTRenderSystem**: Manages rendering pipelines, materials, and visual effects.
- **CLTPhysicsSystem**: Handles collision detection, raycasting, and physical interactions.
- **CLTResourceSystem**: Loads and manages game resources (models, textures, sounds, etc.).
- **CLTNetworkSystem**: Handles client-server communication and packet processing.
- **CLTScriptSystem**: Provides Python and Lua script execution environment.

### Game Objects

- **CLTCharacter**: Base class for all character entities (players and NPCs).
- **CLTPlayer**: Represents a player-controlled character.
- **CLTNPC**: Represents a non-player character.
- **CLTWorldModel**: Represents static world geometry.
- **CLTItem**: Represents collectible or interactive items.

### Game Object Update System

The engine uses a component-based update system for game objects:

- **CLTGameObject::Update(float deltaTime)**: Main update method called each frame that:
  1. Updates physics components for non-static objects
  2. Updates transform components (position, rotation)
  3. Updates visual components (model, animations)
  4. Updates spatial partitioning data
  5. Calls base class update for general object state

- **Component Updates**: The update cascade follows this order:
  1. Physics controller update (collision resolution, forces)
  2. Transform update (position, rotation matrices)
  3. Model update (visual representation, animations)
  4. Spatial node update (position in spatial partitioning system)

- **Flag-Based Behavior Control**: Object behavior is driven by a flag system:
  - VISIBLE: Controls if object is rendered
  - SOLID: Enables collision detection
  - GRAVITY: Makes object affected by gravity
  - STATIC: Disables physics updates
  - INTERACTIVE: Allows player interaction
  - MOVABLE: Permits movement/teleportation
  - CASTS_SHADOW: Enables shadow casting
  - PATHABLE: Allows pathfinding on surfaces

## Memory Management

The engine uses a custom memory management system with specialized allocators:

- **CLTSuballocator**: Pool-based memory allocator for game objects.
- **CLTHeapAllocator**: General-purpose memory allocator.
- **CLTFrameAllocator**: Temporary memory allocator that resets each frame.

## Threading Model

The engine uses a hybrid threading model:

- Main thread: Game logic, physics, and primary rendering operations
- Worker threads: Asset loading, background processing, and some physics calculations
- Network thread: Packet processing and network operations

## Resource Management

Game resources are organized in a hierarchical system:

- Resources are primarily stored in PKB (Package Bundle) files
- The resource system maintains a cache of frequently used resources
- Resources are reference-counted for automatic unloading
- Streaming system loads/unloads resources based on proximity and priority

## Network Protocol

The game uses a custom network protocol with the following characteristics:

- Hybrid TCP/UDP approach
- Sequenced, reliable packet delivery
- Encryption for sensitive data
- Compression for bandwidth optimization
- State synchronization for game objects

### RPC System

The game implements a Remote Procedure Call (RPC) system for client-server communication:

- **Command Dispatching**: Commands are routed through handler maps based on ID size:
  ```cpp
  // Client-side RPC registration and handling
  typedef void (CLTNetworkHandler::*RPCHandler)(CLTByteBuffer &cmdData);
  
  // Maps for byte-sized and short-sized command IDs
  std::map<uint8_t, RPCHandler> m_byteCommands;
  std::map<uint16_t, RPCHandler> m_shortCommands;
  ```

- **Common Client RPC Commands**:
  - Chat message sending
  - Player movement and animation
  - Object interaction
  - Character actions (jump, crouch, etc.)
  - UI events and selections

- **Command Processing Flow**:
  1. Local action triggers command generation
  2. Command ID and parameters are serialized
  3. Command is sent to server via PLAYER_COMMAND message
  4. Server processes command and updates game state
  5. Server sends state updates to relevant clients

## Rendering Pipeline

The rendering system is designed around a flexible pipeline:

- Deferred shading approach
- Material-based rendering
- Level-of-detail (LOD) system
- Occlusion culling
- Post-processing effects

## Animation System

Characters use a sophisticated animation system:

- Skeletal animation with blending
- Procedural animation for certain effects
- Animation state machines
- Inverse kinematics for precise positioning

## Future Extensions

This documentation will be expanded as more of the engine is reconstructed, with particular focus on:

- NavMesh system
- Mission system
- Combat mechanics
- UI framework
- Scripting interfaces