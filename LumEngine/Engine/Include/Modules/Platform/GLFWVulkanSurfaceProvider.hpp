//========= Copyright (C) 2025-present 3zymek, MIT License  ============//
//
// Purpose: GLFW-based OpenGL rendering context implementation.
//
//=============================================================================//
#pragma once

#include "Core/Utils/SafePtr.hpp"
#include "Platform/VulkanSurfaceProvider.hpp"

class GLFWwindow;

namespace lum {

	class GLFWVulkanSurfaceProvider : public IVulkanSurfaceProvider {
	public:

		GLFWVulkanSurfaceProvider( GLFWwindow* window ) : m_Window( window ) {}

		VkSurfaceKHR CreateSurface( VkInstance instance ) const override;

	private:

		SafePtr<GLFWwindow> m_Window = nullptr;

	};

} // namespace lum