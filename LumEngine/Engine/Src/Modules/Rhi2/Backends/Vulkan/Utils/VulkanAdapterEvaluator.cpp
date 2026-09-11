#include "Rhi2/Backends/Vulkan/Utils/VulkanAdapterEvaluator.hpp"
#include "volk.h"

namespace lum::rhi::vk {

	void VulkanAdapterEvaluator::SetRequirements( const AdapterRequirements& req ) noexcept {

		m_Requirements = req;

	}

	VulkanAdapter VulkanAdapterEvaluator::EvaluateAdapter( VkPhysicalDevice device ) const noexcept {

		VulkanAdapter score{};

		VkPhysicalDeviceFeatures features{};
		vkGetPhysicalDeviceFeatures( device, &features );

		VkPhysicalDeviceProperties props{};
		vkGetPhysicalDeviceProperties( device, &props );

		

		for (auto feature : m_Requirements.GetRequiredFeatures( )) {

			auto it = sk_AdapterFeaturesMap.find( feature );
			if (it == sk_AdapterFeaturesMap.end( )) {
				LUM_LOG_FATAL( "Feature isn't mapped" );
				continue;
			}
			
			if (features.*it->second == VK_TRUE) {
				score.m_Features.*it->second = VK_TRUE;
				score.m_Score++;
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
				score.m_Features.*it->second = VK_TRUE;
				score.m_Score += weight;
			}

		}

		return score;

	}

} // namespace lum::rhi::vk