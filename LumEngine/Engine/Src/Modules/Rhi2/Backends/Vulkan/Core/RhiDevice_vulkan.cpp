#include "Rhi2/Backends/Vulkan/Core/RhiDevice_vulkan.hpp"
#include "Rhi2/RhiAdapter.hpp"

namespace lum::rhi::vk {

	void VulkanDevice::Initialize( const RenderDeviceCreateInfo& info ) noexcept {

		if (volkInitialize( ) != VK_SUCCESS) {
			LUM_LOG_FATAL( "Failed to initialize volk (Vulkan loader)" );
		}

		create_vk_instance( info );
		volkLoadInstance( m_Instance );

		choose_adapter( );
		
	}

	void VulkanDevice::create_vk_instance( const RenderDeviceCreateInfo& info ) noexcept {

		std::vector<const char*> validationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};

		std::vector<const char*> extensions = info.m_InstanceExtensions;
		if (info.m_EnableValidation) {
			extensions.push_back( VK_EXT_DEBUG_UTILS_EXTENSION_NAME );
		}

		VkApplicationInfo appInfo{};
		appInfo.apiVersion = VK_MAKE_VERSION( 1, 0, 0 );
		appInfo.engineVersion = VK_MAKE_VERSION( 0, 3, 0 );
		appInfo.pApplicationName = "Blank";
		appInfo.pEngineName = "LumEngine";

		VkInstanceCreateInfo desc{};
		desc.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		desc.pApplicationInfo = &appInfo;
		desc.enabledExtensionCount = SafeCast<uint32>( extensions.size( ) );
		desc.ppEnabledExtensionNames = extensions.data( );

		// ENABLE VALIDATION
		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
		if (info.m_EnableValidation) {
			
			debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

			debugCreateInfo.messageSeverity =
				VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;

			debugCreateInfo.messageType =
				VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

			debugCreateInfo.pfnUserCallback =
				[]( VkDebugUtilsMessageSeverityFlagBitsEXT severity,
					VkDebugUtilsMessageTypeFlagsEXT type,
					const VkDebugUtilsMessengerCallbackDataEXT* data,
					void* userData ) -> VkBool32 {
						if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
							LUM_LOG_ERROR( "[VULKAN VALIDATION] {}", data->pMessage );
						}
						else {
							LUM_LOG_WARN( "[VULKAN VALIDATION] {}", data->pMessage );
						}
						return VK_FALSE;
				};

			desc.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
			desc.enabledLayerCount = SafeCast<uint32>( validationLayers.size( ) );
			desc.ppEnabledLayerNames = validationLayers.data( );

		}

		if (vkCreateInstance( &desc, nullptr, &m_Instance ) != VK_SUCCESS) {
			LUM_LOG_FATAL( "Failed to create VkInstance!" );
		}

	}

	void VulkanDevice::choose_adapter( ) noexcept {

		uint32 numDevices = 0;
		vkEnumeratePhysicalDevices( m_Instance, &numDevices, nullptr );
		if (numDevices <= 0) {
			LUM_LOG_FATAL( "There are no physical devices to be found!" );
			return;
		}
		std::vector<VkPhysicalDevice> devices( numDevices );
		vkEnumeratePhysicalDevices( m_Instance, &numDevices, devices.data( ) );

		VulkanAdapter bestAdapter{};

		for (int32 i = 0; i <= numDevices; i++) {

			VulkanAdapter evaluated = m_AdapterEvaluator.EvaluateAdapter( devices[ i ] );
			
			if (evaluated.m_Score > bestAdapter.m_Score) 
				bestAdapter = evaluated;

		}

		if (bestAdapter.m_Score == 0) {
			LUM_LOG_FATAL( "Failed to initialize Vulkan backend: None of the ({}) detected GPU(s) satisfy LumEngine's minimum required features.", numDevices );
			return;
		}
		
		m_Adapter = bestAdapter;

	}

} // namespace lum::rhi::vk