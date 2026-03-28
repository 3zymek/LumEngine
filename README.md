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

## Contributing
I'm looking for contributors! If you're interested in game engine development and want to learn together, feel free to reach out.

**Discord:** `br.eeze` <br>
**Email:** `szymonpyrtek@gmail.com`

Any help is welcome — bug fixes, new features, or just feedback on the code.

## Project Structure
````
LumEngine/
├──engine
│   ├── core/           # Core engine functionality
│   ├── math/           # Math library and backend
│   ├── scene/          # Scene management (not implemented yet)
│   ├── utils/          # Utility functions/helpers
│   modules/
│   ├── rhi/            # Rendering Hardware Interface
│   │   └── backend/    # OpenGL/Vulkan implementation
│   ├── ahi/            # Audio Hardware Interface
│   │   └── backend/    # FMOD implementation
│   ├── render/         # Render resources managers, renderer
│   ├── audio/          # Audio resources manager
│   ├── entity/         # ECS (Entity Component System)
│   ├── audio/          # Audio system
│   ├── event/          # EBS (Event Bus System)
│   ├── platform/       # Window and input
│   └── scene/          # Scene management
````

### Prefixes
````bash
M (Manager)     - Class that manages data flow and object lifetime
C (Component)   - Component in ECS (Entity Component System)
E (Event)       - Event in EBS (Event Bus System)
R (Render)      - Render Hardware Interface handles
F (Plain Data)  - Stores only data

m (member)  - Variable is a member of a class
s (static)  - Variable is static
g (global)  - Variable doesn't belong to any class (global state)
k (const)   - Variable is constant (or constexpr)
t (template)- Template type/variable

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

## Quality

The code quality varies as I learn. Older parts might not follow current best practices. I'm refactoring as I go and learning better approaches along the way.

Feel free to look around, but keep in mind this is an in-progress learning project!

## License

MIT License - see [LICENSE](LICENSE) file.

This is an educational project. Feel free to learn from it, but don't expect perfect AAA code.
