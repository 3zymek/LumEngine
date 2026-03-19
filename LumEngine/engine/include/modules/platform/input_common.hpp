//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Global input polling system for the Lum engine.
//
//=============================================================================//
#pragma once

#include "core/core_common.hpp"
#include "key_codes.hpp"

namespace lum {

	/* @brief Public input polling API.
	*  Contains all user-facing functions for querying keyboard and mouse state.
	*  Wraps GLFW input calls behind an engine-friendly interface.
	*
	*  @note Must call SetActiveWindow() before any input queries.
	*  @note Not thread-safe — call from the main thread only.
	*/
	namespace input {

		/* @brief Internal state for the input system.
		*  Not intended for direct use outside of the input subsystem.
		*/
		namespace detail {

			/* @brief Active GLFW window used for input polling.
			*  Set via SetActiveWindow() before using any input functions.
			*/
			inline GLFWwindow* gActiveWindow = nullptr;

		} // namespace lum::input::detail

		/* @brief Sets the active window for input polling.
		*  Must be called before any input functions are used.
		*  @param window Pointer to the GLFW window to poll input from.
		*/
		inline void SetActiveWindow( GLFWwindow* window ) {
			detail::gActiveWindow = window;
		}

		/* @brief Returns the GLFW key code for the given engine key.
		*  @param key Engine key to map.
		*  @return GLFW key code.
		*/
		inline uint32 GetGLFWKey( Key key ) {
			LUM_ASSERT( detail::gActiveWindow != nullptr, "Active window isn't set" );
			return detail::gKeyMap[ static_cast< uint32 >(key) ];
		}

		/* @brief Returns true only on the first frame the key is held down.
		*  Subsequent frames while the key is held will return false.
		*  @param key Key to check.
		*  @return True if key was just pressed this frame.
		*/
		inline bool KeyPressedOnce( Key key ) {

			LUM_ASSERT( detail::gActiveWindow != nullptr, "Active window isn't set" );

			bool keyPressed = glfwGetKey( detail::gActiveWindow, GetGLFWKey( key ) ) == GLFW_PRESS;
			int32 keyCode = static_cast< int32 >(key);

			if (keyPressed && !detail::gKeyIsPressed[ keyCode ]) {
				detail::gKeyIsPressed[ keyCode ] = true;
				return true;
			}

			if (!keyPressed)
				detail::gKeyIsPressed[ keyCode ] = false;

			return false;
		}

		/* @brief Returns true every frame the key is held down.
		*  @param key Key to check.
		*  @return True if key is currently pressed.
		*/
		inline bool KeyPressed( Key key ) {
			LUM_ASSERT( detail::gActiveWindow != nullptr, "Active window isn't set" );
			return glfwGetKey( detail::gActiveWindow, GetGLFWKey( key ) ) == GLFW_PRESS;
		}

		/* @brief Returns the current mouse cursor position in screen coordinates.
		*  @return Mouse position as a 2D vector (x, y).
		*/
		inline glm::vec2 GetMousePos( ) {
			LUM_ASSERT( detail::gActiveWindow != nullptr, "Active window isn't set" );
			float64 xpos, ypos;
			glfwGetCursorPos( detail::gActiveWindow, &xpos, &ypos );
			return glm::vec2( xpos, ypos );
		}
	} // namespace lum::input
} // namespace lum