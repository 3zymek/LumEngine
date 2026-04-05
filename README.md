<div align="center">

![lumengine logo](https://raw.githubusercontent.com/3zymek/LumEngine/main/LumEngine/internal_assets/branding/lumengine_medium2.png)

# LumEngine

Learning indie game engine project from scratch in C++.

![License](https://img.shields.io/badge/license-Apache%202.0-blue.svg)
![C++](https://img.shields.io/badge/C%2B%2B-20-blue.svg)
![Status](https://img.shields.io/badge/status-in%20development-orange.svg)
[![wakatime](https://wakatime.com/badge/github/3zymek/LumEngine.svg)](https://wakatime.com/badge/github/3zymek/LumEngine)

</div>

## About

This is my personal project where I'm learning game engine architecture by building one from the ground up. The goal is to understand how modern engines work by implementing core systems myself.

<img width="2549" height="1393" alt="lumengine_shadows" src="https://github.com/user-attachments/assets/17341395-facc-4afe-a0ca-736b556e33eb" />

*Shadow mapping showcase — Mar 8, 2026*

**Status: Early Development** — Many features are incomplete or experimental.

## Contributing

I'm looking for contributors! If you're interested in game engine development and want to learn together, feel free to reach out.

**Discord:** `br.eeze`  
**Email:** `szymonpyrtek@gmail.com`

For more info see [CONTRIBUTING](CONTRIBUTING.md) file.

Any help is welcome — bug fixes, new features, or just feedback on the code.

## Project Structure

```
LumEngine/
├── engine/
│   ├── core/           # Core engine functionality
│   ├── scene/          # Scene management (not implemented yet)
│   └── utils/          # Utility functions/helpers
└── modules/
    ├── rhi/            # Rendering Hardware Interface
    │   └── backend/    # OpenGL implementation
    ├── ahi/            # Audio Hardware Interface
    │   └── backend/    # FMOD implementation
    ├── render/         # Render resources managers, renderer
    ├── audio/          # Audio resources manager
    ├── entity/         # ECS (Entity Component System)
    ├── event/          # EBS (Event Bus System)
    ├── platform/       # Window and input
    └── scene/          # Scene management
```

### Prefixes

```
M  (Manager)          - Class that manages data flow and object lifetime
C  (Component)        - Component in ECS (Entity Component System)
E  (Event)            - Event in EBS (Event Bus System)
R  (Render)           - Render Hardware Interface handles
F  (Plain Data)       - Stores only data
m  (member)           - Variable is a member of a class
s  (static)           - Variable is static
sk (static constexpr) - Variable is static and constexpr
g  (global)           - Variable doesn't belong to any class (global state)
k  (const)            - Variable is constant (or constexpr)
t  (template)         - Template type/variable

I use PascalCase for public functions and classes, snake_case for private methods and camelCase for variables.
```

## Building

### Requirements

- MSVC (Visual Studio 2022)
- CMake 3.20+
- Python 3.x
- .NET SDK (for LHC header compiler)

### Build Steps

```
!!! WORKS ONLY ON MSVC FOR NOW !!!
1. Download and unpack the zip file
2. Run "init.py" in root directory - this will download external packages and generate the project
3. Enter the "build" folder and open the .sln file
```

## Known Issues

This is a learning project, so expect:
- Incomplete features
- Bugs and crashes
- Non-optimized code in places
- Missing error handling in some areas
- Documentation gaps

## Quality

The code quality varies as I learn. Older parts might not follow current best practices, but I try my best to refactor low-quality code.

Feel free to look around, but keep in mind this is an in-progress learning project!

## License

Apache 2.0 — see [LICENSE](LICENSE) file.

This is an educational project. Feel free to learn from it, but don't expect perfect AAA code.
