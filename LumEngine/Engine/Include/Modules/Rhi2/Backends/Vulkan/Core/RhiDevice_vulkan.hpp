#pragma once

#include "Rhi2/Interfaces/Core/RhiDevice.hpp"
#include "Rhi2/Backends/Vulkan/RhiCommon_vulkan.hpp"
#include "Rhi2/Backends/Vulkan/Utils/VulkanAdapterEvaluator.hpp"

namespace lum {

	class IVulkanSurfaceProvider;

} // namespace lum

namespace lum::rhi::vk {

	class VulkanDevice : public IRenderDevice {
	public:

		void Initialize( const RenderDeviceCreateInfo& info ) noexcept override;
		void Finalize( ) noexcept override;

	private:

		void create_vk_instance( const RenderDeviceCreateInfo& info ) noexcept;
		void choose_adapter( ) noexcept;
		void create_logical_device( ) noexcept;
		void create_main_surface( ) noexcept;
		void create_swapchain( TVector2<uint32> windowSize ) noexcept;
		void extract_swapchain_images( ) noexcept;

		VkInstance m_Instance = VK_NULL_HANDLE;

		VulkanAdapterEvaluator m_AdapterEvaluator{};
		VulkanAdapter m_Adapter{};

		VkDevice m_LogicalDevice = VK_NULL_HANDLE;

		SafePtr<IVulkanSurfaceProvider> m_SurfaceProvider = nullptr;
		VkSurfaceKHR m_MainSurface = VK_NULL_HANDLE;

		VkSwapchainKHR m_Swapchain = VK_NULL_HANDLE;
		std::vector<VkImage> m_SwapchainImages{};
		std::vector<VkImageView> m_SwapchainImageViews{};

	};

} // namespace lum::rhi::vk