<div align="center">

![lumengine logo](https://github.com/3zymek/LumEngine/blob/main/LumEngine/assets/branding/lumengine_medium2.png)

# LumEngine

Learning game engine project from scratch in C++.

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![C++](https://img.shields.io/badge/C%2B%2B-20-blue.svg)
![Status](https://img.shields.io/badge/status-in%20development-orange.svg)

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