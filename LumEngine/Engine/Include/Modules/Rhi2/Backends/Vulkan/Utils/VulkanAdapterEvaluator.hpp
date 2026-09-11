#pragma once

#include "Rhi2/Backends/Vulkan/RhiCommon_vulkan.hpp"
#include "Rhi2/RhiAdapter.hpp"

namespace lum::rhi::vk {

	struct VulkanAdapter {
		VkPhysicalDeviceFeatures m_Features{};
		uint32 m_Score = 0;
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