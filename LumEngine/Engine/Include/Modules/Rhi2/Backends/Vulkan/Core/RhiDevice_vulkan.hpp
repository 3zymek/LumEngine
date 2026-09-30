#pragma once

#include "Rhi2/Interfaces/Core/RhiDevice.hpp"
#include "Rhi2/Backends/Vulkan/RhiCommon_vulkan.hpp"
#include "Rhi2/Backends/Vulkan/Utils/VulkanAdapterEvaluator.hpp"
#include "Rhi2/Backends/Vulkan/Core/RhiBuffer_vulkan.hpp"

namespace lum {

	class IVulkanSurfaceProvider;

} // namespace lum

namespace lum::rhi::vk {

	class VulkanDevice : public IRenderDevice {
	public:

		void Initialize( const RenderDeviceCreateInfo& info ) noexcept override;
		void Finalize( ) noexcept override;

		void UpdateFrame( ) noexcept override;

		BufferHandle2 CreateBuffer( const BufferCreateInfo2& info ) noexcept override;
		void DestroyBuffer( BufferHandle2& handle ) noexcept override;
	
	private:

		void assert_device( ) const;

		void create_vk_instance( const RenderDeviceCreateInfo& info ) noexcept;
		void choose_adapter( ) noexcept;
		void create_logical_device( ) noexcept;
		void acquire_queues( ) noexcept;
		void create_shader_stages( ) noexcept;
		void create_main_surface( ) noexcept;
		void recreate_swapchain( ) noexcept;
		void recreate_swapchain_images( ) noexcept;
		void create_main_pipeline( ) noexcept;
		void create_command_pool( ) noexcept;
		void allocate_command_buffers( ) noexcept;
		void create_sync_primitives( ) noexcept;
		void handle_resize( ) noexcept;
		void create_vertex_buffers( ) noexcept;



		static inline constexpr usize sk_MaxBuffers = 256;

		cstd::HandlePool<BufferHandle2, VulkanBuffer, BufferID2> m_Buffers{ sk_MaxBuffers };



		TVector2<uint32> m_WindowSize{};

		VkInstance m_Instance = VK_NULL_HANDLE;

		VulkanAdapterEvaluator m_AdapterEvaluator{};
		VulkanAdapter m_Adapter{};

		VkDevice m_LogicalDevice = VK_NULL_HANDLE;

		SafePtr<IVulkanSurfaceProvider> m_SurfaceProvider = nullptr;
		VkSurfaceKHR m_MainSurface = VK_NULL_HANDLE;

		VkSwapchainKHR m_Swapchain = VK_NULL_HANDLE;
		std::vector<VkImage> m_SwapchainImages{};
		std::vector<VkImageView> m_SwapchainImageViews{};

		VkPipelineLayout m_PipelineLayout = VK_NULL_HANDLE;
		VkPipeline m_MainPipeline = VK_NULL_HANDLE;

		VkCommandPool m_CmdPool = VK_NULL_HANDLE;
		std::vector<VkCommandBuffer> m_CmdBuffers{ LUM_MAX_FRAMES_IN_FLIGHT };

		VkQueue m_GraphicsQueue = VK_NULL_HANDLE;
		VkQueue m_ComputeQueue = VK_NULL_HANDLE;
		VkQueue m_PresentQueue = VK_NULL_HANDLE;
		
		std::array<VkSemaphore, LUM_MAX_FRAMES_IN_FLIGHT> m_ImageAvailableSemaphores{};
		std::vector<VkSemaphore> m_RenderFinishedSemaphores{};

		VkFence m_Fence = VK_NULL_HANDLE;

		VkShaderModule DT_Vertex = VK_NULL_HANDLE;
		VkShaderModule DT_Fragment = VK_NULL_HANDLE;

		uint32 m_CurrentFrame = 0;

		BufferHandle2 DT_Buffer{};

		bool m_Initialized = false;

	};

} // namespace lum::rhi::vk