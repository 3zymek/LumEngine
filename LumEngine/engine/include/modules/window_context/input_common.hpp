#pragma once
#include "key_codes.hpp"
#include "core/core_pch.hpp"

// THIS IS NOT AN CLASS FOR THIS MOMENT BUT WILL BE

namespace lum {
	namespace input {
		namespace detail {
			
			GLFWwindow* active_window = nullptr;
		
		}

		inline void SetActiveWindow(GLFWwindow* window) {
			detail::active_window = window;
		}

		inline uint32 GetGLFWKey( Key key ) {
			assert(detail::active_window && "Active window isn't set");
			return detail::key_map[static_cast<uint32>(key)];
		}

		inline bool KeyPressedOnce( Key key ) {
			assert(detail::active_window && "Active window isn't set");
			bool key_pressed = glfwGetKey(detail::active_window, GetGLFWKey(key)) == GLFW_PRESS;
			int key_code = static_cast<int32>(key);

			if (key_pressed && !detail::key_is_pressed[key_code]) {
				detail::key_is_pressed[key_code] = true;
				return true;
			}
			
			if (!key_pressed)
				detail::key_is_pressed[key_code] = false;
			
			return false;

		}

		inline bool KeyPressed( Key key ) {
			assert(detail::active_window && "Active window isn't set");
			return glfwGetKey(detail::active_window, GetGLFWKey(key)) == GLFW_PRESS;
		}

		inline glm::vec2 GetMousePos( ) {
			assert(detail::active_window && "Active window isn't set");
			double xpos, ypos;
			glfwGetCursorPos(detail::active_window, &xpos, &ypos);
			return glm::vec2(xpos, ypos);
		}
	}
}