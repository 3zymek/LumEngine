#pragma once
#include "key_codes.hpp"
#include "core/core_pch.hpp"
namespace input {
	static GLFWwindow* activeWindow = nullptr;

	inline unsigned int GetGLFWKey(Key key) {
		assert(activeWindow && "Active window isn't set!");
		return static_cast<unsigned int>(key);
	}

	inline bool KeyPressed(Key key) {
		assert(activeWindow && "Active window isn't set!");
		return glfwGetKey(activeWindow, GetGLFWKey(key)) == GLFW_PRESS;
	}

	inline glm::vec2 GetMousePos() {
		assert(activeWindow && "Active window isn't set!");
		double xpos, ypos;
		glfwGetCursorPos(activeWindow, &xpos, &ypos);
		return glm::vec2(xpos, ypos);
	}


}