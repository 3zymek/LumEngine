![lumengine logo](https://github.com/3zymek/LumEngine/blob/main/LumEngine/assets/branding/lumengine_medium2.png)

A learning project - building a game engine from scratch in C++20.

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![C++](https://img.shields.io/badge/C%2B%2B-20-blue.svg)
![Status](https://img.shields.io/badge/status-in%20development-orange.svg)

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

## 📁 Project Structure

LumEngine/
├── core/              # Core engine functionality
│   ├── math/          # Math library and backend
│   ├── scene/         # Scene management ( not implemented yet )
│   └── utils/         # Utility functions/helpers
├── modules/
│   ├── rhi/           # Rendering Hardware Interface
│   │   └── backend/   # OpenGL implementation
│   ├── ecs/           # Entity Component System
│   ├── audio/         # Audio system
│   └── events/        # Event system
└── sandbox/           # Test application

## 🛠️ Building

### Prerequisites

- C++20 compiler (GCC 11+, Clang 14+, MSVC 2022+)
- CMake 3.20+
- OpenGL 4.5+

### Build Steps
```bash
git clone https://github.com/yourusername/LumEngine.git
cd LumEngine
mkdir build && cd build
cmake ..
cmake --build .
```
## Known Issues

This is a learning project, so expect:
- Incomplete features
- Bugs and crashes
- Non-optimized code in places
- Missing error handling in some areas
- Documentation gaps

## Current Focus

1. Finishing ECS implementation
2. Improving asset loading system
3. Adding basic material system
4. Learning and implementing PBR rendering

The code quality varies as I learn. Older parts might not follow current best practices. I'm refactoring as I go and learn better practices.

Feel free to look around, but keep in mind this is a in progress learning project

## 📄 License

MIT License - see [LICENSE](LICENSE) file.

This is an educational project. Feel free to learn from it, but don't expect perfect AAA code.

## 📧 Contact

Questions or suggestions? Feel free to open an issue!

---