/*******************************************************/
/* /                                                 \ */
/*|-------------------|LumEngine|---------------------|*/
/*|            Cross-Platform Game Engine             |*/
/*|===================================================|*/
/*|                    GitHub:                        |*/
/*|       https://github.com/3zymek/LumEngine         |*/
/*|                                                   |*/
/*|        Copyright (c) 2025-present 3zymek          |*/
/*|          Licensed under the MIT License           |*/
/* \                                                 / */
/*******************************************************/
#include "Core/CorePch.hpp"

#include <glfw3.h>

#include "CreateInfo.hpp"
#include "Platform/Input/Input.hpp"
#include "Platform/Window.hpp"
#include "PlatformModule.hpp"
#include "Rhi/Interfaces/Core/RhiDevice.hpp"
#include "Rhi/Backends/Vulkan/Core/RhiDevice_vulkan.hpp"
#include "Platform/GLFWVulkanSurfaceProvider.hpp"

namespace lum {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void PlatformModule::Initialize( EngineCreateInfo& info, ev::EventBus& bus ) {

		info.m_Window.m_Title = info.m_ApplicationName;

		if (!info.m_RenderContext) {

			// Create default window with default LumEngine backends

			m_Window = Window{};
			m_Window.Value( ).Initialize( info.m_Window, bus );

			auto* glfwWindow = static_cast<GLFWwindow*>(m_Window.Value( ).GetNativeWindow( ));

		}
		else {

		}

		m_RenderDevice = OwningPtr<rhi::vk::VulkanDevice>::Create( );

		uint32 count{};
		const char** glfwExtensions = glfwGetRequiredInstanceExtensions( &count );
		std::vector<const char*> extensions( glfwExtensions, glfwExtensions + count );

		OwningPtr<GLFWVulkanSurfaceProvider> surfaceProvider = OwningPtr<GLFWVulkanSurfaceProvider>::Create( reinterpret_cast<GLFWwindow*>(m_Window.Value( ).GetNativeWindow( )) );

		rhi::RenderDeviceCreateInfo deviceInfo{};
		deviceInfo.m_InstanceExtensions = extensions;
		deviceInfo.m_EnableValidation = true;
		deviceInfo.m_SurfaceProvider = surfaceProvider;
		deviceInfo.m_EventBus = bus;

		m_RenderDevice( ).Initialize( deviceInfo );

	}

	void PlatformModule::Finalize( ) {
		m_RenderDevice( ).Finalize( );
	}

} // namespace lum