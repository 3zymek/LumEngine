# Contributing to LumEngine

LumEngine is a solo-developed C++ game engine and I'm always open to contributors who want to help grow the project. If you're interested in contributing, **reach out to me first** — let's talk before you start working on anything.

## Getting in Touch

Open an issue or contact me directly:

**Discord:** `br.eeze`
**GitHub:** [@3zymek](https://github.com/3zymek)

## Reporting Bugs

Open an issue with:
- Description of the bug
- Steps to reproduce
- Expected vs actual behavior
- Your OS and GPU

## Suggesting Features

Open an issue with a clear description of what you want and why it makes sense for the engine.

## Pull Requests

Before opening a PR:
1. Open an issue or contact me to discuss the change
2. Make sure we're aligned on the approach
3. Follow the code style below

I want to stay involved in every decision — the architecture is evolving fast and consistency matters a lot to me.

## Code Style

- `m` prefix for members, `sk` for static constexpr, `b` for booleans
- `F` prefix for plain data structs, `M` for managers, `R` for RHI types
- snake_case for private methods
- No exceptions, no RTTI