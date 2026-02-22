#pragma once
#include "key_codes.hpp"
#include "core/core_pch.hpp"

// ! THIS IS NOT AN CLASS FOR THIS MOMENT BUT WILL BE

namespace lum {
	namespace input {
		namespace detail {
			
			inline GLFWwindow* gActiveWindow = nullptr;
		
		}

		inline void SetActiveWindow(GLFWwindow* window) {
			detail::gActiveWindow = window;
		}

		inline uint32 GetGLFWKey( Key key ) {
			assert(detail::gActiveWindow && "Active window isn't set");
			return detail::gKeyMap[static_cast<uint32>(key)];
		}

		inline bool KeyPressedOnce( Key key ) {
			assert(detail::gActiveWindow && "Active window isn't set");
			bool key_pressed = glfwGetKey(detail::gActiveWindow, GetGLFWKey(key)) == GLFW_PRESS;
			int key_code = static_cast<int32>(key);

			if (key_pressed && !detail::gKeyIsPressed[key_code]) {
				detail::gKeyIsPressed[key_code] = true;
				return true;
			}
			
			if (!key_pressed)
				detail::gKeyIsPressed[key_code] = false;
			
			return false;

		}

		inline bool KeyPressed( Key key ) {
			assert(detail::gActiveWindow && "Active window isn't set");
			return glfwGetKey(detail::gActiveWindow, GetGLFWKey(key)) == GLFW_PRESS;
		}

		inline glm::vec2 GetMousePos( ) {
			assert(detail::gActiveWindow && "Active window isn't set");
			double xpos, ypos;
			glfwGetCursorPos(detail::gActiveWindow, &xpos, &ypos);
			return glm::vec2(xpos, ypos);
		}
	}
}