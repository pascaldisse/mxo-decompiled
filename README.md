# Matrix Online Decompiled

This repository contains decompiled and reconstructed source code from The Matrix Online client, based on reverse engineering analysis using radare2. The code is provided for educational purposes only.

## Disclaimer

This is an unofficial, fan-made reconstruction of the game's code based on reverse engineering. It is not the original source code and is likely incomplete or may contain inaccuracies. This code is provided for educational and research purposes only.

The Matrix Online and all related intellectual property are owned by Warner Bros. Entertainment Inc. This project is not affiliated with or endorsed by Warner Bros. or any of the original developers.

## Project Status - May 2025 Update

We have begun a more comprehensive analysis of the Matrix Online executable using radare2. The current focus is on:

1. Core engine classes (CLTBaseClass implementation completed)
2. Navigation mesh system (partially analyzed)
3. Network system classes (identified but not yet decompiled)

A complete decompilation plan is available in the `DECOMPILATION_PLAN.md` file at the repository root.

## Project Structure

The repository is organized into the following main directories:

- `src/` - Main source code
  - `core/` - Core engine components
  - `rendering/` - Rendering system
  - `network/` - Network code
  - `gameplay/` - Gameplay mechanics
  - `ui/` - User interface code
- `include/` - Header files
- `lib/` - Library interfaces
- `docs/` - Additional documentation

## Key Classes

- **CLTBaseClass**: Root class for all engine objects, providing RTTI, reference counting, and basic lifecycle management
- **CLTObject**: Base class for interactive objects, adding property support and message handling
- **CLTGameObject**: Base class for game world entities, adding position, rotation, and collision
- **CLTNavMeshSystem**: Handles navigation meshes for character movement and pathfinding

## Key Systems Analyzed

- **Game Object Update System**: Component-based update cascade with physics, transform, model, and spatial updates
- **RPC Command System**: Client-side remote procedure call mechanism for sending commands to server
- **Flag-Based Behavior**: Objects use flag system (VISIBLE, SOLID, GRAVITY, etc.) to control behavior

## Important Addresses Identified

| Address    | Function/Class          | Description                        |
|------------|-------------------------|------------------------------------|
| 0x621729a0 | IsIndoors              | Determines if position is indoors   |
| 0x6266971c | FindPathToo            | Main pathfinding algorithm         |
| 0x004b035c | CLTLoginMediator       | Login system entry point           |

## Building

This code is not intended to be built into a functional game. It is a reference implementation for educational purposes only.

## License

This project is provided for educational purposes under fair use principles. All copyrights and trademarks are the property of their respective owners.