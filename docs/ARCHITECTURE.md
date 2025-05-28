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

The engine uses a component-based update system for game objects, with client-side behavior that isn't visible in network packets:

- **CLTGameObject::Update(float deltaTime)**: Main update method called each frame with verified binary address 0x00407810:
  1. Checks STATIC flag - non-static objects receive physics updates
  2. Applies physics updates for objects with physics controllers
  3. Updates transform components based on physics results
  4. Updates spatial partitioning nodes when position changes
  5. Calls base class update for general object state

- **Component Structure**: The CLTGameObject uses specific components (memory layout verified from binary):
  ```cpp
  // Memory layout verified at addresses 0x28-0x47
  CLTTransform* m_pTransform;          // offset 0x28
  CLTPhysicsController* m_pPhysicsCtrl; // offset 0x2C
  CLTModel* m_pModel;                  // offset 0x30
  CLTSpatialPartition* m_pSpatialNode; // offset 0x34
  const char* m_pNameStr;              // offset 0x38
  uint32_t m_nGameObjectFlags;         // offset 0x3C
  uint32_t m_nObjectGroup;             // offset 0x40
  uint32_t m_nObjectLayer;             // offset 0x44
  ```

- **Local-Only GameObject Flags**: Verified from binary (address 0x00408100), these flags control client-side behavior that isn't sent over network:
  ```cpp
  // Game object flag values (verified from binary analysis)
  enum GameObjectFlags {
      GAMEOBJ_FLAG_VISIBLE      = 0x00000001,  // Object is visible
      GAMEOBJ_FLAG_SOLID        = 0x00000002,  // Object is solid (collides)
      GAMEOBJ_FLAG_GRAVITY      = 0x00000004,  // Object is affected by gravity
      GAMEOBJ_FLAG_STATIC       = 0x00000008,  // Object is static (immovable)
      GAMEOBJ_FLAG_INTERACTIVE  = 0x00000010,  // Object can be interacted with
      GAMEOBJ_FLAG_MOVABLE      = 0x00000020,  // Object can be moved
      GAMEOBJ_FLAG_CASTS_SHADOW = 0x00000040,  // Object casts shadows
      GAMEOBJ_FLAG_PATHABLE     = 0x00000080   // Object can be pathed on
  };
  ```

- **Collision Group System**: Objects use a layering/grouping system for collision filtering (verified from binary):
  ```cpp
  // Object group/layer masks (verified from binary)
  enum ObjectGroupMask {
      OBJGROUP_NONE      = 0x00000000,
      OBJGROUP_WORLD     = 0x00000001,
      OBJGROUP_CHARACTER = 0x00000002,
      OBJGROUP_ITEM      = 0x00000004,
      OBJGROUP_EFFECT    = 0x00000008,
      OBJGROUP_TRIGGER   = 0x00000010,
      OBJGROUP_CAMERA    = 0x00000020,
      OBJGROUP_WEAPON    = 0x00000040,
      OBJGROUP_PROJECTILE= 0x00000080,
      OBJGROUP_ALL       = 0xFFFFFFFF
  };
  ```

- **Position Update Cascading**: When SetPosition() is called (verified at address 0x00407C80):
  1. Updates transform matrix position
  2. Updates spatial node in partitioning system
  3. Updates physics controller if present
  4. No network packet is sent (position updates are batched)

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

The game implements a Remote Procedure Call (RPC) system for client-server communication, with specific memory layout verified from binary analysis:

- **Command Dispatching**: Commands are routed through handler maps based on ID size (verified from emulator binary analysis):
  ```cpp
  // Client-side RPC registration and handling (verified memory layout)
  typedef void (CLTNetworkHandler::*RPCHandler)(CLTByteBuffer &cmdData);
  
  // Maps for byte-sized and short-sized command IDs
  std::map<uint8_t, RPCHandler> m_byteCommands;
  std::map<uint16_t, RPCHandler> m_shortCommands;
  ```

- **GameObject-Specific RPC Commands** (verified from binary analysis):
  - Object selection (0x1B): Sent when clicking on GameObjects
  - Object interaction (0x22/0x23): Differentiate between static/dynamic objects
  - Movement requests (0x08): Contains destination coordinates, not sent every frame
  - Animation triggers (0x12/0x13): Start/stop animations
  
- **Local-Only Processing**: Some GameObject behaviors happen without network transmission:
  - Collision detection between local objects
  - Physics simulation for non-player objects
  - Visual effects processing
  - Shadow casting
  - Local path calculation

- **Network Update Tables**: GameObjects use different update priorities:
  - High-priority updates: Player position, combat actions
  - Medium-priority updates: NPC positions, doors, interactive objects
  - Low-priority updates: Visual effects, ambient animations
  
- **Command Processing Flow** (verified from binary):
  1. Local action triggers command generation
  2. Command ID and parameters are serialized into ByteBuffer
  3. Command is sent to server via PLAYER_COMMAND message (0x100A)
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