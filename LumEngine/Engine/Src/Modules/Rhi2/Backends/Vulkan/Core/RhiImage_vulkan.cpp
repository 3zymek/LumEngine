#include "Rhi2/Backends/Vulkan/Core/RhiDevice_vulkan.hpp"
#include "Rhi2/Backends/Vulkan/RhiEnumCast_vulkan.hpp"

namespace lum::rhi::vk {

	ImageHandle2 VulkanDevice::CreateImage( const ImageCreateInfo2& info ) noexcept {

		if (m_Images.IsFull( )) {
			LUM_LOG_WARN( "Couldn't create RHI Image: Max images reached! Increase max images count or refactor your code." );
			return {};
		}

		auto image = m_ImageCreator.CreateImage( info );
		if (!image) {
			LUM_LOG_ERROR( "Failed to create RHI Image: {}", image.GetError( ) );
			return {};
		}

		return m_Images.Append( image.ValueRef( ) );

	}
	void VulkanDevice::DestroyImage( ImageHandle2& handle ) noexcept {

		if (!m_Images.Contains( handle )) return;

		auto& image = m_Images[ handle ];
		vkDeviceWaitIdle( m_LogicalDevice );

		if (image.m_Image != VK_NULL_HANDLE)
			vkDestroyImage( m_LogicalDevice, image.m_Image, nullptr );
		if (image.m_View != VK_NULL_HANDLE)
			vkDestroyImageView( m_LogicalDevice, image.m_View, nullptr );

		m_Images.Remove( handle );

	}

} // namespace lum::rhi::vk