//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Platform window abstraction for the Lum Engine.
//
//=============================================================================//
#pragma once

#include "core/core_common.hpp"

namespace lum {
	namespace ev { class EventBus; }

	/* @brief Descriptor used to configure a window on creation. */
	struct WindowDescriptor {
		String mTitle = "LumEngine"; /* @brief Window title bar text. */
		uint32 mHeight = 500;         /* @brief Initial window height in pixels. */
		uint32 mWidth = 500;         /* @brief Initial window width in pixels. */
		ev::EventBus* mEventBus = nullptr;     /* @brief Event bus to emit window events to. */
	};

	/* @brief Platform window wrapping a GLFW window.
	*  Responsible for window creation, size tracking and emitting
	*  window events (resize, close) through the EventBus.
	*
	*  @note Create via CreateWindow() rather than constructing directly.
	*/
	class Window {
	public:

		/* @brief Initializes the window with the given descriptor.
		*  Creates the underlying GLFW window and registers it with the event bus.
		*  @param desc Configuration for the window.
		*/
		void Initialize( const WindowDescriptor& desc );

		/* @brief Sets the window width in pixels. */
		void SetWidth( uint32 );

		/* @brief Sets the window height in pixels. */
		void SetHeight( uint32 );

		/* @brief Returns the current window width in pixels. */
		uint32 GetWidth( ) const noexcept;

		/* @brief Returns the current window height in pixels. */
		uint32 GetHeight( ) const noexcept;

		/* @brief Returns the underlying native GLFW window pointer. */
		vptr GetNativeWindow( ) const noexcept;

		/* @brief Polls window events and emits EWindowResized if size changed.
		*  Call once per frame from the main loop.
		*/
		void Update( ) noexcept;

		/* @brief Returns true if the window is still open. */
		bool IsOpen( ) const noexcept;

		float64 GetDeltaTime( ) const noexcept { return glfwGetTime( ); }

	protected:

		GLFWwindow* mWindow = nullptr; /* @brief Underlying GLFW window handle. */
		ev::EventBus* mEventBus = nullptr; /* @brief Event bus for window events. */

		uint32 mWidth = 0;
		uint32 mHeight = 0;

		/* @brief Internal initialization called by Initialize(). */
		void init( const WindowDescriptor& desc );
	};

} // namespace lum