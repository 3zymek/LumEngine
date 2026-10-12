#include "Rhi2/Backends/Vulkan/Core/RhiDevice_vulkan.hpp"
#include "Rhi2/Backends/Vulkan/RhiEnumCast_vulkan.hpp"

namespace lum::rhi::vk {
	 
	ImageHandle2 VulkanDevice::CreateImage( const ImageCreateInfo2& info ) noexcept {

		VkExtent3D extent{};
		extent.width = info.m_Extent.m_Width;
		extent.height = info.m_Extent.m_Height;
		extent.depth = info.m_Extent.m_Depth;

		VkImageCreateInfo vkInfo{};
		vkInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		vkInfo.usage = ToVk( info.m_UsageFlags );
		vkInfo.format = ToVk( info.m_Format );
		vkInfo.imageType = ToVk( info.m_Type );
		vkInfo.extent = extent;
		vkInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		vkInfo.mipLevels = info.m_MipmapLevels;
		vkInfo.arrayLayers = info.m_ArrayLayers;
		vkInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		vkInfo.initialLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		
		VkImage image{};

		const VkResult result = vkCreateImage( m_LogicalDevice, &vkInfo, nullptr, &image );
		if (result != VK_SUCCESS) {
			LUM_LOG_ERROR( "Couldn't create image: (VkResult: {})", static_cast<uint32>(result) );
			return {};
		}



	}
	void VulkanDevice::DestroyImage( ImageHandle2& handle ) noexcept {

	}

} // namespace lum::rhi::vk