//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Platform window abstraction for the Lum engine.
//
//=============================================================================//

#include "platform/window.hpp"
#include "event/event_bus.hpp"
#include "core/utils/lum_assert.hpp"
#include "event/events/window_events.hpp"
#include "platform/input_common.hpp"

namespace lum {

	void Window::Initialize( const WindowDescriptor& desc ) {

		mEventBus = desc.mEventBus;
		init( desc );

	}

	void Window::SetWidth( uint32 width ) {

		mWidth = width;
		glfwSetWindowSize( mWindow, mWidth, mHeight );

	}
	void Window::SetHeight( uint32 height ) {

		mHeight = height;
		glfwSetWindowSize( mWindow, mWidth, mHeight );

	}
	uint32 Window::GetWidth( ) const noexcept {

		if (!mWindow) return 0;
		int32 w, h;
		glfwGetWindowSize( mWindow, &w, &h );
		return w;

	}

	uint32 Window::GetHeight( ) const noexcept {

		if (!mWindow) return 0;
		int32 w, h;
		glfwGetWindowSize( mWindow, &w, &h );
		return h;

	}

	void Window::ToggleDecoration( bool value ) {
		if (bDecorations != value) {
			glfwSetWindowAttrib( mWindow, GLFW_DECORATED, value ? GLFW_TRUE : GLFW_FALSE );
			bDecorations = value;
		}
	}
	void Window::ToggleResizable( bool value ) { 
		if (bResizable != value) {
			glfwSetWindowAttrib( mWindow, GLFW_RESIZABLE, value ? GLFW_TRUE : GLFW_FALSE );
			bResizable = value;
		}
	}
	void Window::ToggleFloating( bool value ) { 
		if (bFloating != value) {
			glfwSetWindowAttrib( mWindow, GLFW_FLOATING, value ? GLFW_TRUE : GLFW_FALSE );
			bFloating = value;
		}
	}
	void Window::ToggleVisibility( bool value ) { 
		if (bVisible != value) {
			value ? glfwShowWindow( mWindow ) : glfwHideWindow( mWindow );
			bVisible = value;
		}
	}

	void Window::Update( ) noexcept {

		int32 width, height;
		glfwGetWindowSize( mWindow, &width, &height );
		if (width != mWidth || height != mHeight) {
			EWindowResized ev;

			mWidth = width;
			mHeight = height;

			ev.mWidth = width;
			ev.mHeight = height;

			mEventBus->Emit( ev );
		}

		for (uint32 i = 0; i < ArraySize( input::detail::gKeyMap ); i++) {

			bool keyPressed = glfwGetKey( static_cast< GLFWwindow* >( mWindow ), input::GetGLFWKey( input::Key( i ) ) ) == GLFW_PRESS;

			if (keyPressed && !input::detail::gKeyIsPressed[ i ]) mEventBus->Emit( EKeyPressed{ .mKey = ( input::Key ) i } );
			if (!keyPressed && input::detail::gKeyIsPressed[ i ]) mEventBus->Emit( EKeyReleased{ .mKey = ( input::Key ) i } );

			input::detail::gKeyIsPressed[ i ] = keyPressed;
		}

		glfwPollEvents( );

	}

	void Window::init( const WindowDescriptor& desc ) {

		if (!glfwInit( )) {
			LUM_LOG_FATAL( "Failed to initialize GLFW" );
			return;
		}

		if (desc.mFlags.Has( WindowInitFlags::NoResize )) {
			glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
			bResizable = false;
		}
		if (desc.mFlags.Has( WindowInitFlags::NoDecoration )) {
			glfwWindowHint( GLFW_DECORATED, GLFW_FALSE );
			bDecorations = false;
		}
		if (desc.mFlags.Has( WindowInitFlags::Floating )) {
			glfwWindowHint( GLFW_FLOATING, GLFW_TRUE );
			bFloating = true;
		}
		if (desc.mFlags.Has( WindowInitFlags::CenterCursor ))
			glfwWindowHint( GLFW_CENTER_CURSOR, GLFW_TRUE );
		if (desc.mFlags.Has( WindowInitFlags::Maximized )) {
			glfwWindowHint( GLFW_MAXIMIZED, GLFW_TRUE );
			bMaximized = true;
		}
		if (desc.mFlags.Has( WindowInitFlags::Focused ))
			glfwWindowHint( GLFW_FOCUSED, GLFW_TRUE );
		if (desc.mFlags.Has( WindowInitFlags::Invisible )) {
			glfwWindowHint( GLFW_VISIBLE, GLFW_FALSE );
			bVisible = false;
		}

		mWindow = glfwCreateWindow( desc.mWidth, desc.mHeight, desc.mTitle.c_str( ), nullptr, nullptr );

		mWidth = desc.mWidth;
		mHeight = desc.mHeight;

		if (!mWindow) {
			glfwTerminate( );
			return;
		}

	}

}