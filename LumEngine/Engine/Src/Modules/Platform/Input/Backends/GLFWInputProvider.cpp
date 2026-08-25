#include "Platform/Input/Backends/GLFWInputProvider.hpp"

namespace lum::input {

	bool GLFWInputProvider::KeyPressed( Key key ) {

		if (key == Key::MOUSE_LEFT || key == Key::MOUSE_RIGHT || key == Key::MOUSE_MIDDLE) {
			return glfwGetMouseButton( mNativeWindow.Ptr( ), GetNativeKey( key ) ) == GLFW_PRESS;
		}
		return glfwGetKey( mNativeWindow.Ptr( ), GetNativeKey( key ) ) == GLFW_PRESS;

	}

	bool GLFWInputProvider::KeyPressedOnce( Key key ) {

		bool keyPressed = KeyPressed( key );
		int32 keyCode = ToUnderlyingEnum( key );

		if (keyPressed && !detail::pressedKeysArray[ keyCode ]) {
			detail::pressedKeysArray[ keyCode ] = true;
			return true;
		}
		
		if (!keyPressed)
			detail::pressedKeysArray[ keyCode ] = false;

		return false;

	}

	Vector2 GLFWInputProvider::GetMousePosition( ) const {
		float64 x{}, y{};
		glfwGetCursorPos( mNativeWindow.Ptr( ), &x, &y );
		return Vector2( ToFloat32( x ), ToFloat32( y ) );
	}

}