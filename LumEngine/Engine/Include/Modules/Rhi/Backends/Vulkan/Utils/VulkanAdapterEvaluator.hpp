#pragma once

#include <unordered_map>
#include <vector>

#include <vulkan/vulkan_core.h>

#include "Core/CoreTypes.hpp"
#include "Core/Utils/Result.hpp"
#include "Rhi/Backends/Vulkan/VulkanAdapter.hpp"
#include "Rhi/Interfaces/RhiAdapter.hpp"

namespace lum::rhi::vk {

	class VulkanAdapterEvaluator {
	public:

		void SetRequirements( const AdapterRequirements& req ) noexcept;
		Result<VulkanAdapter> EvaluateAdapter( VkPhysicalDevice device, VkSurfaceKHR surface ) const noexcept;

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