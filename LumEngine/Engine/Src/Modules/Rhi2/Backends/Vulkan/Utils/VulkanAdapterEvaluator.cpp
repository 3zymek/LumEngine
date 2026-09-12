#include "Rhi2/Backends/Vulkan/Utils/VulkanAdapterEvaluator.hpp"
#include "volk.h"

namespace lum::rhi::vk {

	void VulkanAdapterEvaluator::SetRequirements( const AdapterRequirements& req ) noexcept {

		m_Requirements = req;

	}

	VulkanAdapter VulkanAdapterEvaluator::EvaluateAdapter( VkPhysicalDevice device ) const noexcept {

		VulkanAdapter adapter{};

		VkPhysicalDeviceFeatures features{};
		vkGetPhysicalDeviceFeatures( device, &features );

		VkPhysicalDeviceProperties props{};
		vkGetPhysicalDeviceProperties( device, &props );

		VkPhysicalDeviceMemoryProperties memProps{};
		vkGetPhysicalDeviceMemoryProperties( device, &memProps );

		if (memProps.memoryHeapCount > 0) {

			for (usize i = 0; i < memProps.memoryHeapCount; i++) {
				if (memProps.memoryHeaps[ i ].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
					adapter.m_Desc.m_DedicatedVideoMemory += SafeCast<usize>( memProps.memoryHeaps[ i ].size );
				}
				else {
					adapter.m_Desc.m_SharedSystemMemory += SafeCast<usize>( memProps.memoryHeaps[ i ].size );
				}
			}

		}

		uint32 numQueueFamilies = 0;
		vkGetPhysicalDeviceQueueFamilyProperties( device, &numQueueFamilies, nullptr );
		std::vector<VkQueueFamilyProperties> queueFamilies( numQueueFamilies );
		vkGetPhysicalDeviceQueueFamilyProperties( device, &numQueueFamilies, queueFamilies.data( ) );

		for (uint32 i = 0; i < numQueueFamilies; i++) {

			auto queueFlags = queueFamilies[ i ].queueFlags;

			if (queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				if (!adapter.m_Queues.HasQueue( adapter.m_Queues.m_GraphicsQueueIndex )) {
					adapter.m_Queues.m_GraphicsQueueIndex = i;
				}
			}
			if (queueFlags & VK_QUEUE_COMPUTE_BIT) {
				if (!adapter.m_Queues.HasQueue( adapter.m_Queues.m_ComputeQueueIndex )) {
					adapter.m_Queues.m_ComputeQueueIndex = i;
				}
			}

		}

		adapter.m_Desc.m_AdapterName = props.deviceName;
		adapter.m_Desc.m_AdapterId = props.deviceID;
		adapter.m_Desc.m_VendorId = props.vendorID;

		for (auto feature : m_Requirements.GetRequiredFeatures( )) {

			auto it = sk_AdapterFeaturesMap.find( feature );
			if (it == sk_AdapterFeaturesMap.end( )) {
				LUM_LOG_FATAL( "Feature isn't mapped" );
				continue;
			}

			if (features.*it->second == VK_TRUE) {
				adapter.m_Features.*it->second = VK_TRUE;
				adapter.m_Score++;
			}
			else {
				return {};
			}

		}

		for (auto [feature, weight] : m_Requirements.GetFeatures( )) {

			auto it = sk_AdapterFeaturesMap.find( feature );
			if (it == sk_AdapterFeaturesMap.end( )) {
				LUM_LOG_WARN( "Feature isn't mapped" );
				continue;
			}

			if (features.*it->second == VK_TRUE) {
				adapter.m_Features.*it->second = VK_TRUE;
				adapter.m_Score += weight;
			}

		}

		return adapter;

	}

} // namespace lum::rhi::vk