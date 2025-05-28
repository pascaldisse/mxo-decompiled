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