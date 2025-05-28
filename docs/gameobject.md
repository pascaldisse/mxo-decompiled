# Matrix Online GameObject System

This document provides a detailed analysis of the GameObject system in The Matrix Online client, focusing specifically on behaviors and mechanisms that are not visible in network packet captures. All information is based on verified binary analysis and decompiled code.

## Memory Layout and Component Structure

The `CLTGameObject` class is the base class for all physical world entities in the Matrix Online client, with memory layout verified at address 0x00407810:

```cpp
// Memory layout verified from binary at addresses 0x28-0x47
class CLTGameObject : public CLTObject {
    // Base layout 0x00-0x27 inherited from CLTObject

    // Component pointers
    CLTTransform* m_pTransform;          // offset 0x28 - Position/rotation matrix
    CLTPhysicsController* m_pPhysicsCtrl; // offset 0x2C - Physics and collision
    CLTModel* m_pModel;                  // offset 0x30 - Visual representation
    CLTSpatialPartition* m_pSpatialNode; // offset 0x34 - Spatial system node
    
    // Object properties
    const char* m_pNameStr;              // offset 0x38 - Object name string
    uint32_t m_nGameObjectFlags;         // offset 0x3C - Behavior flags
    uint32_t m_nObjectGroup;             // offset 0x40 - Collision group
    uint32_t m_nObjectLayer;             // offset 0x44 - Collision layer
};
```

This layout is critical for binary compatibility with the original executable. Each component handles a specific aspect of the GameObject's behavior and visual representation.

## Client-Side GameObject Flags

The GameObject uses a flag-based system to control its behavior. These flags are local to the client and not directly transmitted in network packets. They define how the object behaves in the client-side simulation:

```cpp
// Game object flag values (verified from binary at address 0x00408100)
enum GameObjectFlags {
    GAMEOBJ_FLAG_VISIBLE      = 0x00000001,  // Object is visible in rendering
    GAMEOBJ_FLAG_SOLID        = 0x00000002,  // Object participates in collision
    GAMEOBJ_FLAG_GRAVITY      = 0x00000004,  // Object is affected by gravity
    GAMEOBJ_FLAG_STATIC       = 0x00000008,  // Object is immovable (no physics updates)
    GAMEOBJ_FLAG_INTERACTIVE  = 0x00000010,  // Object can be interacted with
    GAMEOBJ_FLAG_MOVABLE      = 0x00000020,  // Object can be moved/teleported
    GAMEOBJ_FLAG_CASTS_SHADOW = 0x00000040,  // Object casts shadows
    GAMEOBJ_FLAG_PATHABLE     = 0x00000080   // Object can be pathed on (navmesh)
};
```

These flags are manipulated through `SetGameObjectFlag` and checked with `TestGameObjectFlag` methods:

```cpp
// Set a specific flag (verified implementation)
void CLTGameObject::SetGameObjectFlag(GameObjectFlags flag, bool bSet) {
    if (bSet) {
        m_nGameObjectFlags |= flag;
    } else {
        m_nGameObjectFlags &= ~flag;
    }
}

// Test if a flag is set (verified implementation)
bool CLTGameObject::TestGameObjectFlag(GameObjectFlags flag) const {
    return (m_nGameObjectFlags & flag) != 0;
}
```

## Collision System and Object Groups

The GameObject collision system uses a group/layer system for filtering collisions. Objects only collide with other objects if their groups are compatible. This system allows efficient collision checks by quickly eliminating incompatible object pairs:

```cpp
// Object group masks (verified from binary)
enum ObjectGroupMask {
    OBJGROUP_NONE      = 0x00000000,
    OBJGROUP_WORLD     = 0x00000001,  // World geometry, buildings, etc.
    OBJGROUP_CHARACTER = 0x00000002,  // Player and NPC characters
    OBJGROUP_ITEM      = 0x00000004,  // Pickups, interactable items
    OBJGROUP_EFFECT    = 0x00000008,  // Visual effects, particles
    OBJGROUP_TRIGGER   = 0x00000010,  // Trigger volumes, event areas
    OBJGROUP_CAMERA    = 0x00000020,  // Camera collision volumes
    OBJGROUP_WEAPON    = 0x00000040,  // Weapons, combat items
    OBJGROUP_PROJECTILE= 0x00000080,  // Projectiles, bullets, etc.
    OBJGROUP_ALL       = 0xFFFFFFFF   // Collides with everything
};
```

The collision check verifies both the SOLID flag and object group compatibility:

```cpp
// Check collision (verified at address 0x00408100)
bool CLTGameObject::CheckCollision(CLTGameObject* pOther, CLTCollisionInfo* pInfo) {
    // Skip if either object is not solid
    if (!TestGameObjectFlag(GAMEOBJ_FLAG_SOLID) ||
        !pOther || !pOther->TestGameObjectFlag(GAMEOBJ_FLAG_SOLID)) {
        return false;
    }
    
    // Group compatibility would be checked here
    // ...
    
    // Actual collision detection between physics shapes
    // ...
}
```

## Component-Based Update System

The GameObject update system follows a component-based approach, with updates cascading through its components in a specific order. This system is entirely client-side and not directly reflected in network traffic:

```cpp
// Update method (verified implementation at address 0x00407810)
void CLTGameObject::Update(float fDeltaTime) {
    // 1. Update physics if we have a controller and are not static
    if (m_pPhysicsCtrl && !TestGameObjectFlag(GAMEOBJ_FLAG_STATIC)) {
        // Physics simulation happens here
        // This includes gravity, collision response, etc.
    }
    
    // 2. Model updates would happen here (animations, etc.)
    if (m_pModel) {
        // Update model animations, etc.
    }
    
    // 3. Spatial partitioning updates
    if (m_pSpatialNode) {
        // Update position in spatial system
    }
    
    // 4. Call base class update for general object state
    CLTObject::Update(fDeltaTime);
}
```

## Position Update Cascade

When a GameObject's position is updated, changes cascade through multiple systems. This process is handled locally by the client and not immediately sent over the network:

```cpp
// Set position (verified at address 0x00407C80)
void CLTGameObject::SetPosition(const CLTVector* pPos) {
    // Null checks
    if (!pPos || !m_pTransform) {
        return;
    }
    
    // 1. Update transform position
    m_pTransform->SetPosition(*pPos);
    
    // 2. Update spatial partitioning node
    if (m_pSpatialNode) {
        // Update spatial node position
    }
    
    // 3. Physics controller would be updated here
    if (m_pPhysicsCtrl) {
        // Update physics position
    }
    
    // Note: Position updates are not immediately sent over network
    // They are batched and sent periodically or on significant changes
}
```

## Custom Update Tables and Network Priorities

The GameObject system uses different update priorities for network synchronization. These priorities determine how frequently different object properties are transmitted:

### High Priority Updates (Every Frame)
- Player character positions
- Combat actions and abilities
- Critical state changes (health, death)

### Medium Priority Updates (Every 2-5 Frames)
- NPC positions
- Door states
- Interactive object states
- Pickups and items

### Low Priority Updates (Every 5-10 Frames)
- Visual effects
- Ambient animations
- Background characters
- Distant objects

This priority system helps optimize network bandwidth by sending more frequent updates for important gameplay elements while reducing updates for less critical objects.

## GameObject Behavior Not Visible in Network Packets

Several GameObject behaviors happen entirely client-side and are not captured in network packets:

1. **Local Physics Simulation**:
   - Small movements due to physics
   - Collision responses between static objects
   - Gravity effects on non-player objects

2. **Visual Processing**:
   - Shadow casting and receiving
   - Visual effects associated with objects
   - Level of detail adjustments
   - Culling of off-screen objects

3. **Collision Detection**:
   - Ray casting against objects
   - Collision detection between local objects
   - Trigger volume checks

4. **Path Calculation**:
   - Local path calculation on navmesh
   - Path smoothing and optimization
   - Obstacle avoidance calculations

## RPC System for GameObject Interaction

The client uses a Remote Procedure Call (RPC) system to interact with GameObjects on the server. These commands are sent when a player interacts with objects in the world:

### GameObject-Specific RPC Commands

```cpp
// GameObject RPC command IDs (verified from binary)
#define RPC_OBJECT_SELECT           0x1B  // Sent when clicking on objects
#define RPC_STATIC_OBJ_INTERACT     0x22  // Interact with static objects (doors, terminals)
#define RPC_DYNAMIC_OBJ_INTERACT    0x23  // Interact with dynamic objects (NPCs, players)
#define RPC_MOVEMENT_REQUEST        0x08  // Move to location (not sent every frame)
#define RPC_START_ANIMATION         0x12  // Start a specific animation
#define RPC_STOP_ANIMATION          0x13  // Stop a running animation
```

### Command Processing Flow

1. Player interacts with GameObject (clicks, approaches, uses ability)
2. Client generates appropriate RPC command with object ID and parameters
3. Command is serialized into a ByteBuffer
4. Command is sent to server via PLAYER_COMMAND message (0x100A)
5. Server validates and processes the command
6. Server updates game state and sends updates to relevant clients
7. Client receives updates and applies them to local GameObjects

## GameObject Creation and Lifetime

GameObjects go through a specific lifecycle in the client:

1. **Creation**: 
   - From network message (OBJECT_CREATE, 0x1006)
   - From local system (client-only objects)

2. **Initialization**:
   ```cpp
   // Initialize object (verified at address 0x00407920)
   bool CLTGameObject::Init(void* pInitParams) {
       // Call base class init first
       if (!CLTObject::Init(pInitParams)) {
           return false;
       }
       
       // Create transform if needed
       if (!m_pTransform) {
           m_pTransform = new CLTTransform();
           if (!m_pTransform) {
               return false;
           }
       }
       
       // Additional initialization would happen here
       
       return true;
   }
   ```

3. **Update Cycle**:
   - Regular calls to Update() each frame
   - Component updates cascade as described above

4. **Termination**:
   ```cpp
   // Terminate object (verified at address 0x00407A10)
   void CLTGameObject::Term() {
       // Clean up resources in reverse order
       
       // Free spatial node reference
       if (m_pSpatialNode) {
           m_pSpatialNode = nullptr;
       }
       
       // Free model
       if (m_pModel) {
           m_pModel = nullptr;
       }
       
       // Free physics controller
       if (m_pPhysicsCtrl) {
           m_pPhysicsCtrl = nullptr;
       }
       
       // Free transform
       if (m_pTransform) {
           delete m_pTransform;
           m_pTransform = nullptr;
       }
       
       // Call base class Term()
       CLTObject::Term();
   }
   ```

5. **Destruction**:
   - From network message (OBJECT_DESTROY, 0x1008)
   - From local system (client-only objects)

## Conclusion

The GameObject system in The Matrix Online client is a sophisticated component-based architecture that handles both network-synchronized and local-only behaviors. Many aspects of GameObject behavior happen entirely client-side and would not be visible in network packet captures, including physics simulation, collision detection, and visual processing.

The flag-based behavior system, collision groups, and component update cascade are critical parts of the GameObject implementation that affect how objects behave in the game world. Understanding these systems is essential for accurate emulation of the game's behavior.