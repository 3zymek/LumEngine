//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Platform window abstraction for the Lum Engine.
//
//=============================================================================//
#pragma once
#include "Core/Types.hpp"
#include "Core/Utils/Flags.hpp"
#include "Rhi/RhiPch.hpp"
#include "Core/Utils/ResourceLoader.hpp"

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
		NoCursor		= 1 << 7
	};
	LUM_ENABLE_ENUM_BITFLAG_OPERATIONS( lum::WindowInitFlags );

	/* @brief Bitmask flags representing toggleable runtime states of a window.
	*  Use with Window::ToggleState() to enable or disable individual states.
	*/
	enum class WindowStateFlags : bitfield {
		Decoration	= 1 << 0, // Native OS title bar and border are visible.
		Resizable	= 1 << 1, // Window can be resized by the user.
		Floating	= 1 << 2, // Window stays always on top of other windows.
		Visible		= 1 << 3, // Window is visible on screen.
		Maximized	= 1 << 4, // Window fills the monitor work area.
		Focused		= 1 << 5, // Window has input focus.
		Cursor		= 1 << 6, // Cursor is visible inside the window.
	};
	LUM_ENABLE_ENUM_BITFLAG_OPERATIONS( lum::WindowStateFlags );

	/* @brief Descriptor used to configure a window on creation. */
	struct WindowCreateInfo {
		Flags<WindowInitFlags>		mFlags = {};
		String						mTitle = "LumEngine";		/* @brief Window title bar text. */
		std::optional<ImageData>	mIconData = std::nullopt;	/* @brief Optional loaded icon texture data. */
		uint32						mHeight = 500;				/* @brief Initial window height in pixels. */
		uint32						mWidth = 500;				/* @brief Initial window width in pixels. */
		ev::EventBus*				mEventBus = nullptr;		/* @brief Event bus to emit window events to. */
	};

	/* @brief Platform window wrapping a GLFW window.
	*  Responsible for window creation, size tracking and emitting
	*  window events (resize, close) through the EventBus.
	*/
	class Window {
	public:

		/* @brief Initializes the window with the given descriptor.
		*  Creates the underlying GLFW window and registers it with the event bus.
		*  @param desc Configuration for the window.
		*/
		void Initialize( const WindowCreateInfo& desc );

		/* @brief Sets the window width in pixels. */
		void SetWidth( uint32 width );

		/* @brief Sets the window height in pixels. */
		void SetHeight( uint32 height );

		/* @brief Returns the current window width in pixels. */
		uint32 GetWidth( ) const noexcept;

		/* @brief Returns the current window height in pixels. */
		uint32 GetHeight( ) const noexcept;

		/* @brief Returns the underlying native GLFW window pointer. */
		vptr GetNativeWindow( ) const noexcept { return mWindow; }

		/* @brief Returns the time in seconds since GLFW was initialized. */
		float64 GetTime( ) const noexcept { return glfwGetTime( ); }

		void ToggleState( Flags<WindowStateFlags> flag, bool value );

		/* @brief Polls window events and emits EWindowResized if size changed.
		*  Call once per frame from the main loop.
		*/
		void Update( ) noexcept;

		/* @brief Returns true if the window is still open. */
		bool IsOpen( ) const noexcept { return !glfwWindowShouldClose( mWindow ); }

		/* @brief Signals the window to close on the next Update() call. */
		void Close( ) const { glfwSetWindowShouldClose( mWindow, true ); }

		/* @brief Minimizes the window to the taskbar. */
		void Minimize( ) const { glfwIconifyWindow( mWindow ); }


	protected:

		GLFWwindow* mWindow = nullptr; /* @brief Underlying GLFW window handle. */
		ev::EventBus* mEventBus = nullptr; /* @brief Event bus for window events. */

		uint32 mWidth = 0; /* @brief Current width of window. */
		uint32 mHeight = 0; /* @brief Current height of window. */

		Flags<WindowStateFlags> mState = {};

		using WindowStateToggleFn = void(*)(GLFWwindow* window, int32 value);
		static constexpr WindowStateToggleFn skWindowStateHandlers[ ] = {
			[]( GLFWwindow* w, int32 v ) { glfwSetWindowAttrib( w, GLFW_DECORATED,  v ); },
			[]( GLFWwindow* w, int32 v ) { glfwSetWindowAttrib( w, GLFW_RESIZABLE,  v ); },
			[]( GLFWwindow* w, int32 v ) { glfwSetWindowAttrib( w, GLFW_FLOATING,   v ); },
			[]( GLFWwindow* w, int32 v ) { v ? glfwShowWindow( w ) : glfwHideWindow( w ); },
			[]( GLFWwindow* w, int32 v ) { v ? glfwMaximizeWindow( w ) : glfwRestoreWindow( w ); },
			[]( GLFWwindow* w, int32 v ) { if (v) glfwFocusWindow( w ); },
			[]( GLFWwindow* w, int32 v ) { glfwSetInputMode( w, GLFW_CURSOR, v ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED ); },
		};
		static constexpr uint32 skWindowStateHandlersSize = ArraySize( skWindowStateHandlers );

		/* @brief Internal initialization called by Initialize().
		*  @param desc Configuration descriptor forwarded from Initialize().
		*/
		void init( const WindowCreateInfo& desc );
	};

} // namespace lum