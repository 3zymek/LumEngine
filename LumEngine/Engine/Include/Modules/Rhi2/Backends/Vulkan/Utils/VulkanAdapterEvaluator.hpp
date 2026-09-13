#pragma once

#include "Rhi2/Backends/Vulkan/RhiCommon_vulkan.hpp"
#include "Rhi2/RhiAdapter.hpp"

namespace lum::rhi::vk {

	struct VulkanSurfaceSupport {
		VkSurfaceCapabilitiesKHR		m_Capabilities{};
		std::vector<VkSurfaceFormatKHR> m_Formats{};
		std::vector<VkPresentModeKHR>	m_PresentModes{};
	};

	class VulkanQueueFamilies {
	public:

		uint32 m_GraphicsQueueIndex = MaxValue<uint32>( );
		uint32 m_PresentQueueIndex = MaxValue<uint32>( ); // IMPLEMENT PRESENT QUEUE INDEX
		uint32 m_ComputeQueueIndex = MaxValue<uint32>( );

		LUM_NODISCARD constexpr bool HasQueue( uint32 index ) const noexcept {
			return index != MaxValue<uint32>( );
		}

		LUM_NODISCARD constexpr bool IsValid( ) const noexcept {
			return HasQueue( m_GraphicsQueueIndex ) && HasQueue( m_PresentQueueIndex );
		}

	};

	class VulkanAdapter {
	public:

		VkPhysicalDevice			m_Device = VK_NULL_HANDLE;
		VkPhysicalDeviceFeatures	m_Features{};
		VulkanSurfaceSupport		m_SurfaceSupport{};
		VulkanQueueFamilies			m_Queues{};
		AdapterDescription			m_Desc{};
		uint32						m_Score = 0;

		void QuerySurfaceCapabilities( VkSurfaceKHR surface ) noexcept;

	};

	class VulkanAdapterEvaluator {
	public:

		void SetRequirements( const AdapterRequirements& req ) noexcept;
		VulkanAdapter EvaluateAdapter( VkPhysicalDevice device ) const noexcept;

	private:

		AdapterRequirements m_Requirements{};

		using FeatureByteOffset = VkBool32 VkPhysicalDeviceFeatures::*;
		static inline const std::unordered_map<AdapterFeature, FeatureByteOffset> sk_AdapterFeaturesMap = {
			{ AdapterFeature::Anisotropy, &VkPhysicalDeviceFeatures::samplerAnisotropy },
			{ AdapterFeature::GeometryShader, &VkPhysicalDeviceFeatures::geometryShader },
			{ AdapterFeature::TessellationShader, &VkPhysicalDeviceFeatures::tessellationShader },
			{ AdapterFeature::IndependentBlend, &VkPhysicalDeviceFeatures::independentBlend },
			{ AdapterFeature::MultiViewport, &VkPhysicalDeviceFeatures::multiViewport },
			{ AdapterFeature::WideLines, &VkPhysicalDeviceFeatures::wideLines },
			{ AdapterFeature::SampleRateShading, &VkPhysicalDeviceFeatures::sampleRateShading },
		};

	};

} // namespace lum::rhi::vk