//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: GLFW implementation of the engine input provider interface.
//
//=============================================================================//

#include "Platform/Input/Backends/GLFWInputProvider.hpp"

namespace lum::input {

	//=======================================================//
	// Public
	//=======================================================//

	bool GLFWInputProvider::KeyPressed( Key key ) {

		if (key == Key::MouseLeft || key == Key::MouseRight || key == Key::MouseMiddle) {
			return glfwGetMouseButton( m_NativeWindow.Ptr( ), GetNativeKey( key ) ) == GLFW_PRESS;
		}
		return glfwGetKey( m_NativeWindow.Ptr( ), GetNativeKey( key ) ) == GLFW_PRESS;

	}

	bool GLFWInputProvider::KeyPressedOnce( Key key ) {

		bool keyPressed = KeyPressed( key );
		int32 keyCode = ToUnderlyingEnum( key );

		if (keyPressed && !k_PressedKeysArray[ keyCode ]) {
			k_PressedKeysArray[ keyCode ] = true;
			return true;
		}
		
		if (!keyPressed)
			k_PressedKeysArray[ keyCode ] = false;

		return false;

	}

	Vector2 GLFWInputProvider::GetMousePosition( ) const {
		float64 x{}, y{};
		glfwGetCursorPos( m_NativeWindow.Ptr( ), &x, &y );
		return Vector2( SafeCast<float32>( x ), SafeCast<float32>( y ) );
	}

}