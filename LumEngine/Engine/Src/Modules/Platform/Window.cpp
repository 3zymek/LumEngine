//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Platform window abstraction for the Lum engine.
//
//=============================================================================//

#include "Platform/Window.hpp"
#include "Event/EventBus.hpp"
#include "Core/Utils/LumAssert.hpp"
#include "Event/Events/WindowEvents.hpp"
#include "Platform/Input/Input.hpp"

namespace lum {

	void Window::Initialize( const WindowCreateInfo& desc, ev::EventBus& bus ) {

		m_EventBus = &bus;
		init( desc );

	}

	void Window::SetWidth( uint32 width ) {

		m_Width = width;
		glfwSetWindowSize( m_Window, m_Width, m_Height );

	}
	void Window::SetHeight( uint32 height ) {

		m_Height = height;
		glfwSetWindowSize( m_Window, m_Width, m_Height );

	}
	uint32 Window::GetWidth( ) const noexcept {

		if (!m_Window) return 0;
		int32 w, h;
		glfwGetWindowSize( m_Window, &w, &h );
		return w;

	}

	uint32 Window::GetHeight( ) const noexcept {

		if (!m_Window) return 0;
		int32 w, h;
		glfwGetWindowSize( m_Window, &w, &h );
		return h;

	}

	void Window::ToggleState( Flags<WindowStateFlags> flags, bool value ) {
		for (uint32 bit = 0; bit < sk_WindowStateHandlersSize; bit++) {
			WindowStateFlags flag = static_cast<WindowStateFlags>( 1u << bit );
			if (flags.Has( flag ) && (value != m_State.Has( flag ))) {
				sk_WindowStateHandlers[ bit ]( m_Window, value );
				m_State.Invert( flag );
			}
		}
	}

	void Window::Update( ) noexcept {

		int32 width, height;
		glfwGetWindowSize( m_Window, &width, &height );
		if (width != m_Width || height != m_Height) {

			EWindowResized ev{};

			m_Width = width;
			m_Height = height;

			ev.m_Width = width;
			ev.m_Height = height;

			m_EventBus( ).Emit( ev );

		}

		/*
		for (uint32 i = 0; i < ArraySize( input::detail::kKeyMap ); i++) {

			bool keyPressed = glfwGetKey( static_cast< GLFWwindow* >( m_Window ), input::GetGLFWKey( input::Key( i ) ) ) == GLFW_PRESS;

			if (keyPressed && !input::detail::keyIsPressed[ i ]) m_EventBus->Emit( EKeyPressed{ .m_Key = ( input::Key ) i } );
			if (!keyPressed && input::detail::keyIsPressed[ i ]) m_EventBus->Emit( EKeyReleased{ .m_Key = ( input::Key ) i } );

			input::detail::keyIsPressed[ i ] = keyPressed;
		}
		*/

		glfwPollEvents( );

	}

	void Window::init( const WindowCreateInfo& desc ) {

		if (!glfwInit( )) {
			LUM_LOG_FATAL( "Failed to initialize GLFW" );
			return;
		}

		if (desc.m_Flags.Has( WindowInitFlags::NoResize ))
			glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );

		if (desc.m_Flags.Has( WindowInitFlags::NoDecoration ))
			glfwWindowHint( GLFW_DECORATED, GLFW_FALSE );

		if (desc.m_Flags.Has( WindowInitFlags::Floating ))
			glfwWindowHint( GLFW_FLOATING, GLFW_TRUE );

		if (desc.m_Flags.Has( WindowInitFlags::CenterCursor ))
			glfwWindowHint( GLFW_CENTER_CURSOR, GLFW_TRUE );

		if (desc.m_Flags.Has( WindowInitFlags::Maximized ))
			glfwWindowHint( GLFW_MAXIMIZED, GLFW_TRUE );

		if (desc.m_Flags.Has( WindowInitFlags::Focused ))
			glfwWindowHint( GLFW_FOCUSED, GLFW_TRUE );

		if (desc.m_Flags.Has( WindowInitFlags::Invisible ))
			glfwWindowHint( GLFW_VISIBLE, GLFW_FALSE );

		m_Window = glfwCreateWindow( desc.m_Width, desc.m_Height, desc.m_Title.c_str( ), nullptr, nullptr );

		if (desc.m_IconData) {

			const auto& icon = desc.m_IconData.Value( );

			GLFWimage image;
			image.pixels = const_cast<unsigned char*>(icon.m_Pixels.data( ));
			image.height = icon.m_Height;
			image.width = icon.m_Width;

			glfwSetWindowIcon( m_Window, 1, &image );

		}

		m_Width = desc.m_Width;
		m_Height = desc.m_Height;

		if (!m_Window) {
			glfwTerminate( );
			return;
		}

	}

}