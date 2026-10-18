//========= Copyright (C) 2025-present 3zymek, MIT License  ============//
//
// Purpose: Abstract OpenGL rendering context interface.
//
//=============================================================================//
#pragma once

#include <Vulkan/vulkan_core.h>

#include "Platform/SurfaceProvider.hpp"

namespace lum {

	class IVulkanSurfaceProvider : public ISurfaceProvider {
	public:

		virtual VkSurfaceKHR CreateSurface( VkInstance instance ) const = 0;

	};

} // namespace lum