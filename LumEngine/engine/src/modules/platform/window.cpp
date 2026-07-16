//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Platform window abstraction for the Lum engine.
//
//=============================================================================//

#include "Platform/Window.hpp"
#include "Event/EventBus.hpp"
#include "Core/Utils/LumAssert.hpp"
#include "Event/Events/WindowEvents.hpp"
#include "Platform/InputCommon.hpp"

namespace lum {

	void Window::Initialize( const WindowCreateInfo& desc ) {

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

	void Window::ToggleState( Flags<WindowStateFlags> flags, bool value ) {
		for (uint32 bit = 0; bit < skWindowStateHandlersSize; bit++) {
			WindowStateFlags flag = static_cast< WindowStateFlags >( 1u << bit );
			if (flags.Has( flag ) && (value != mState.Has( flag ))) {
				skWindowStateHandlers[ bit ]( mWindow, value );
				mState.Invert( flag );
			}
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

		for (uint32 i = 0; i < ArraySize( input::detail::kKeyMap ); i++) {

			bool keyPressed = glfwGetKey( static_cast< GLFWwindow* >( mWindow ), input::GetGLFWKey( input::Key( i ) ) ) == GLFW_PRESS;

			if (keyPressed && !input::detail::keyIsPressed[ i ]) mEventBus->Emit( EKeyPressed{ .mKey = ( input::Key ) i } );
			if (!keyPressed && input::detail::keyIsPressed[ i ]) mEventBus->Emit( EKeyReleased{ .mKey = ( input::Key ) i } );

			input::detail::keyIsPressed[ i ] = keyPressed;
		}

		glfwPollEvents( );

	}

	void Window::init( const WindowCreateInfo& desc ) {

		if (!glfwInit( )) {
			LUM_LOG_FATAL( "Failed to initialize GLFW" );
			return;
		}

		if (desc.mFlags.Has( WindowInitFlags::NoResize ))
			glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );

		if (desc.mFlags.Has( WindowInitFlags::NoDecoration ))
			glfwWindowHint( GLFW_DECORATED, GLFW_FALSE );

		if (desc.mFlags.Has( WindowInitFlags::Floating ))
			glfwWindowHint( GLFW_FLOATING, GLFW_TRUE );

		if (desc.mFlags.Has( WindowInitFlags::CenterCursor ))
			glfwWindowHint( GLFW_CENTER_CURSOR, GLFW_TRUE );

		if (desc.mFlags.Has( WindowInitFlags::Maximized ))
			glfwWindowHint( GLFW_MAXIMIZED, GLFW_TRUE );

		if (desc.mFlags.Has( WindowInitFlags::Focused ))
			glfwWindowHint( GLFW_FOCUSED, GLFW_TRUE );

		if (desc.mFlags.Has( WindowInitFlags::Invisible ))
			glfwWindowHint( GLFW_VISIBLE, GLFW_FALSE );

		mWindow = glfwCreateWindow( desc.mWidth, desc.mHeight, desc.mTitle.c_str( ), nullptr, nullptr );

		if (desc.mIconData.has_value( )) {

			const auto& icon = desc.mIconData.value( );

			GLFWimage image;
			image.pixels = const_cast< unsigned char* >(icon.mPixels.data( ));
			image.height = icon.mHeight;
			image.width = icon.mWidth;

			glfwSetWindowIcon( mWindow, 1, &image );

		}

		mWidth = desc.mWidth;
		mHeight = desc.mHeight;

		if (!mWindow) {
			glfwTerminate( );
			return;
		}

	}

}