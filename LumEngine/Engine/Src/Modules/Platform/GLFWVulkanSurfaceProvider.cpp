//========= Copyright (C) 2025-present 3zymek, MIT License  ============//
//
// Purpose: GLFW-based OpenGL rendering context implementation.
//
//=============================================================================//

#include "Platform/GLFWVulkanSurfaceProvider.hpp"

namespace lum {
	
	VkSurfaceKHR GLFWVulkanSurfaceProvider::CreateSurface( VkInstance instance ) const {

		VkSurfaceKHR surface = VK_NULL_HANDLE;
		if (glfwCreateWindowSurface( instance, &m_Window( ), nullptr, &surface ) != VK_SUCCESS) {
			LUM_LOG_FATAL( "Failed to create VkSurfaceKHR!" );
			return VK_NULL_HANDLE;
		}
		return surface;
		
	}

}