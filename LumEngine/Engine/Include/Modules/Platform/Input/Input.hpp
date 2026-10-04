//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Global input polling system for the Lum engine.
//
//=============================================================================//
#pragma once

#include "Platform/Input/InputProvider.hpp"

namespace lum {

	using Key = input::Key;

	/* @brief Public input polling API.
	*  Provides engine-wide functions for querying the current state of
	*  keyboard and mouse input.
	*
	*  The input system delegates platform-specific input handling to an
	*  IInputProvider implementation, keeping the public API independent
	*  of the underlying windowing system.
	*
	*  @note An input provider must be configured before calling any
	*  input query functions.
	*  @note Not thread-safe — intended to be used from the main thread only.
	*/
	namespace input {

		/* @brief Internal implementation details of the input system.
		*  Contains the currently active input provider and functions used
		*  to configure it.
		*
		*  @note This namespace is not intended for direct use outside
		*  of the input subsystem.
		*/
		namespace detail {

			inline SafePtr<IInputProvider> g_InputProvider = nullptr;

			/* @brief Sets the input provider used by the global input API.
			*  @param provider Input provider implementation to use.
			*
			*  @note The provider must remain valid for as long as it is
			*  used by the input system.
			*/
			inline void SetInputProvider( IInputProvider& provider ) noexcept {
				g_InputProvider = provider;
			}

		} // namespace lum::input::detail

		/* @brief Checks whether a key is currently held down.
		*  @param key Key to query.
		*  @return True if the key is currently pressed, otherwise false.
		*/
		inline bool KeyPressed( Key key ) noexcept {
			return detail::g_InputProvider( ).KeyPressed( key );
		}

		/* @brief Checks whether a key was pressed during the current input state.
		*  @param key Key to query.
		*  @return True if the key has been pressed, otherwise false.
		*
		*  @note Unlike KeyPressed(), this function is intended to detect
		*  a single key press rather than continuous input.
		*/
		inline bool KeyPressedOnce( Key key ) noexcept {
			return detail::g_InputProvider( ).KeyPressedOnce( key );
		}

		/* @brief Returns the current mouse cursor position.
		*  @return Mouse position in window coordinates.
		*/
		inline Vector2 GetMousePosition( ) {
			return detail::g_InputProvider( ).GetMousePosition( );
		}

	} // namespace lum::input

} // namespace lum