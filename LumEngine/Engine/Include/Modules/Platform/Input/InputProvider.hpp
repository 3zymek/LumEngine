//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Interface for platform-specific input providers.
//
//=============================================================================//
#pragma once

#include "Core/CoreCommon.hpp"
#include "Platform/Input/KeyCodes.hpp"

namespace lum::input {

    /* @brief Abstract interface for platform-specific input handling.
     *  Implemented by platform layers to provide keyboard and mouse input.
     */
    class IInputProvider {
    public:

        virtual ~IInputProvider( ) = default;

        /* @brief Checks whether a key is currently pressed. */
        virtual bool KeyPressed( Key key ) = 0;

        /* @brief Checks whether a key was pressed once. */
        virtual bool KeyPressedOnce( Key key ) = 0;

        /* @brief Returns the current mouse position. */
        virtual Vector2 GetMousePosition( ) const = 0;

    };

} // namespace lum::input