#include "Rhi2/Backends/Vulkan/Core/RhiDevice_vulkan.hpp"
#include "Rhi2/Backends/Vulkan/RhiEnumCast_vulkan.hpp"

namespace lum::rhi::vk {

	ImageHandle2 VulkanDevice::CreateImage(const ImageCreateInfo2& info) noexcept {

		if (m_Images.IsFull()) {
			LUM_LOG_WARN("Couldn't create RHI Image: Max buffers reached! Increase max images count or refactor your code.");
			return {};
		}



	}
	void VulkanDevice::DestroyImage(ImageHandle2& handle) noexcept {

	}

} // namespace lum::rhi::vk