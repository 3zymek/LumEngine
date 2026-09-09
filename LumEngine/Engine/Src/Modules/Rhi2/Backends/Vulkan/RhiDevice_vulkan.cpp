#include "Rhi2/Backends/Vulkan/RhiDevice_vulkan.hpp"
#include "volk.h"

namespace lum::rhi::vk {

	void VulkanDevice::Initialize( ) noexcept {

		if (volkInitialize( ) != VK_SUCCESS) {
			
		}

	}

} // namespace lum::rhi::vk