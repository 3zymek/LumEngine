<div align="center">

![lumengine logo](https://github.com/3zymek/LumEngine/blob/main/LumEngine/internal_assets/branding/lumengine_medium2.png)

# LumEngine

Learning indie game engine project from scratch in C++.

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![C++](https://img.shields.io/badge/C%2B%2B-20-blue.svg)
![Status](https://img.shields.io/badge/status-in%20development-orange.svg)
[![wakatime](https://wakatime.com/badge/github/3zymek/LumEngine.svg)](https://wakatime.com/badge/github/3zymek/LumEngine)

</div>

## About

This is my personal project where I'm learning game engine architecture by building one from the ground up. The goal is to understand how modern engines work by implementing core systems myself.

**Status: Early Development** - Many features are incomplete or experimental.

## Currently Implemented

- OpenGL backend with state caching
- Basic rendering pipeline
- Entity Component System (in progress)
- Event system
- Audio system (basic)
- Logging and profiling utilities

## Work in Progress

- Fundamental renderer
- Clean code
- Asset management system

## Project Structure
````
LumEngine/
├──engine
│   ├── core/              # Core engine functionality
│   ├── math/          # Math library and backend
│   ├── scene/         # Scene management (not implemented yet)
│   ├── utils/         # Utility functions/helpers
│   modules/
│   ├── rhi/           # Rendering Hardware Interface
│   │   └── backend/   # OpenGL implementation
│   ├── ecs/           # Entity Component System
│   ├── audio/         # Audio system
│   ├── events/        # Event system
│   testfield/         # Testing field (dirty classes etc.)
````

# My Time spent on developing
![time](https://wakatime.com/share/@3zymek/7a14911b-2a12-44ae-984a-4b45b47d27a9.svg)

### Prefixes
````bash
M (Manager)     - Class that manages data flow and object lifetime
S (System)      - Class that processes data each frame, no ownership
C (Component)   - Struct that is component in ECS
L (Lumen)       - Engine types used by user ( or highend )
R (Render)      - Class that is part of Render Hardware Interface
F (Plain Data)  - Stores only data
E (Enum)        - Enum or enum class
I (Interface)   - Abstract class

m (member)  - Variable is a member of a class
s (static)  - Variable is static
g (global)  - Variable doesn't belong to any class (global state)
k (const)   - Variable is constant (or constexpr)

I use PascalCase for public function and classes, snake_case for private methods and camelCase for variables
````

## Building

### Build Steps
````bash
!!! WORKS ONLY ON MSVC FOR NOW !!!

1. Download and unpack the zip file
2. Run "init.py" in root directory - this will install external packages
3. Build folder will show up, enter and run the .sln file
````

## Known Issues

This is a learning project, so expect:

- Incomplete features
- Bugs and crashes
- Non-optimized code in places
- Missing error handling in some areas
- Documentation gaps

## Current Quests

1. Finishing ECS implementation
2. Improving asset loading system
3. Adding basic material system
4. Learning and implementing PBR rendering

The code quality varies as I learn. Older parts might not follow current best practices. I'm refactoring as I go and learning better approaches along the way.

Feel free to look around, but keep in mind this is an in-progress learning project!

## License

MIT License - see [LICENSE](LICENSE) file.

This is an educational project. Feel free to learn from it, but don't expect perfect AAA code.
