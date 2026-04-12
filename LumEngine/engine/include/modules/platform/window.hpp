//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Platform window abstraction for the Lum Engine.
//
//=============================================================================//
#pragma once
#include "core/types.hpp"
#include "core/utils/flags.hpp"
#include "rhi/rhi_pch.hpp"

namespace lum {

	namespace ev { class EventBus; }

	// Flags controlling window initialization behavior.
	enum class WindowInitFlags : bitfield {
		NoDecoration	= 1 << 0, // Removes the native OS title bar and border.
		NoResize		= 1 << 1, // Prevents the user from resizing the window.
		Invisible		= 1 << 2, // Window is hidden on creation, show manually later.
		Maximized		= 1 << 3, // Window starts maximized to fill the work area.
		Floating		= 1 << 4, // Window stays always on top of other windows.
		Focused			= 1 << 5, // Window receives input focus immediately on creation.
		CenterCursor	= 1 << 6, // Cursor is centered in the window on creation.
	};
	LUM_ENUM_OPERATIONS( lum::WindowInitFlags );

	/* @brief Descriptor used to configure a window on creation. */
	struct WindowDescriptor {
		Flags<WindowInitFlags> mFlags = {};
		String mTitle = "LumEngine";			/* @brief Window title bar text. */
		uint32 mHeight = 500;					/* @brief Initial window height in pixels. */
		uint32 mWidth = 500;					/* @brief Initial window width in pixels. */
		ev::EventBus* mEventBus = nullptr;		/* @brief Event bus to emit window events to. */
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
		vptr GetNativeWindow( ) const noexcept { return mWindow; }

		float64 GetTime( ) const noexcept { return glfwGetTime( ); }

		void ToggleDecoration( bool value );
		void ToggleResizable( bool value );
		void ToggleFloating( bool value );
		void ToggleVisibility( bool value );

		/* @brief Polls window events and emits EWindowResized if size changed.
		*  Call once per frame from the main loop.
		*/
		void Update( ) noexcept;

		/* @brief Returns true if the window is still open. */
		bool IsOpen( ) const noexcept { return !glfwWindowShouldClose( mWindow ); }

		void Close( ) const { glfwSetWindowShouldClose( mWindow, true ); }
		void Minimalize( ) const { glfwIconifyWindow( mWindow ); }

	protected:

		GLFWwindow* mWindow = nullptr; /* @brief Underlying GLFW window handle. */
		ev::EventBus* mEventBus = nullptr; /* @brief Event bus for window events. */

		bool bVisible = true;
		bool bResizable = true;
		bool bDecorations = true;
		bool bFloating = false;
		bool bMaximized = false;

		uint32 mWidth = 0;
		uint32 mHeight = 0;

		/* @brief Internal initialization called by Initialize(). */
		void init( const WindowDescriptor& desc );
	};

} // namespace lum