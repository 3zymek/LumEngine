#pragma once

#include <array>
#include <queue>
#include <span>
#include <vector>

#include <vulkan/vulkan_core.h>

#include "Core/CoreDefines.hpp"
#include "Core/CoreTypes.hpp"
#include "Core/Math/Vec2.hpp"
#include "Core/Utils/HandlePool.hpp"
#include "Core/Utils/SafePtr.hpp"
#include "Rhi/Backends/Vulkan/Core/RhiBuffer_vulkan.hpp"
#include "Rhi/Backends/Vulkan/Core/RhiImage_vulkan.hpp"
#include "Rhi/Backends/Vulkan/Core/RhiPipeline_vulkan.hpp"
#include "Rhi/Backends/Vulkan/Core/RhiSampler_vulkan.hpp"
#include "Rhi/Backends/Vulkan/RhiTypes_vulkan.hpp"
#include "Rhi/Backends/Vulkan/Utils/CommandInterpreter.hpp"
#include "Rhi/Backends/Vulkan/Utils/VulkanAdapterEvaluator.hpp"
#include "Rhi/Interfaces/Core/RhiBuffer.hpp"
#include "Rhi/Interfaces/Core/RhiCommandBuffer.hpp"
#include "Rhi/Interfaces/Core/RhiDevice.hpp"
#include "Rhi/Interfaces/Core/RhiImage.hpp"
#include "Rhi/Interfaces/Core/RhiPipeline.hpp"
#include "Rhi/Interfaces/Core/RhiSampler.hpp"
#include "Rhi/RhiTypes.hpp"

namespace lum {

	class IVulkanSurfaceProvider;
	class Path;

} // namespace lum

namespace lum::rhi::vk {

	class LUM_API VulkanDevice : public IRenderDevice {
	public:

		void Initialize( const RenderDeviceCreateInfo& info ) noexcept override;
		void Finalize( ) noexcept override;

		void UpdateFrame( ) noexcept override;

		BufferHandle CreateBuffer( const BufferCreateInfo2& info ) noexcept override;
		void DestroyBuffer( BufferHandle& handle ) noexcept override;

		PipelineHandle CreatePipeline( const PipelineCreateInfo& info ) noexcept override;
		void DestroyPipeline( PipelineHandle& handle ) noexcept override;

		ImageHandle CreateImage( const ImageCreateInfo& info ) noexcept override;
		void DestroyImage( ImageHandle& handle ) noexcept override;

		SamplerHandle CreateSampler( const SamplerCreateInfo& info ) noexcept override;
		void DestroySampler( SamplerHandle& handle ) noexcept override;

		void SendCommandBuffer( CommandBuffer&& buffer ) noexcept override;

	private:

		static inline constexpr usize s_MaxBuffers = 256;
		static inline constexpr usize s_MaxPipelines = 32;
		static inline constexpr usize s_MaxImages = 128;
		static inline constexpr usize s_MaxSamplers = 32;

		HandlePool<
			BufferHandle,
			VulkanBuffer,
			BufferID> m_Buffers{ s_MaxBuffers };

		HandlePool<
			PipelineHandle,
			VulkanPipeline,
			PipelineID> m_Pipelines{ s_MaxPipelines };

		HandlePool<
			ImageHandle,
			VulkanImage,
			ImageID> m_Images{ s_MaxImages };

		HandlePool<
			SamplerHandle,
			VulkanSampler,
			SamplerID> m_Samplers{ s_MaxSamplers };




		VulkanCommandInterpreter m_CommandInterpreter{};
		std::queue<CommandBuffer> m_CommandBuffersToFlush{};

		TVector2<uint32> m_WindowSize{};

		VulkanAdapterEvaluator m_AdapterEvaluator{};
		VulkanAdapter m_Adapter{};

		SafePtr<IVulkanSurfaceProvider> m_SurfaceProvider = nullptr;

		VulkanPipeline m_MainPipeline{};

		uint32 m_CurrentFrame = 0;

		bool m_IsInitialized = false;




		// ===========================================================
		// DEBUG
		// ===========================================================

		inline static std::vector<uint32> DT_Indices = {
			0, 1, 2,
			2, 3, 0
		};
		BufferHandle DT_VertexBuffer{};
		BufferHandle DT_IndexBuffer{};

		VkDescriptorSetLayout DT_DescriptorSetLayout{};
		VkDescriptorPool DT_DescriptorPool{};
		VkDescriptorSet DT_DescriptorSet{};

		ImageHandle DT_Image{};
		SamplerHandle DT_Sampler{};



		// ===========================================================
		// VULKAN SPECIFIC 
		// ===========================================================

		VkInstance m_Instance = VK_NULL_HANDLE;
		VkDevice m_LogicalDevice = VK_NULL_HANDLE;
		VkSurfaceKHR m_MainSurface = VK_NULL_HANDLE;
		VkSwapchainKHR m_Swapchain = VK_NULL_HANDLE;

		std::vector<VkImage> m_SwapchainImages{};
		std::vector<VkImageView> m_SwapchainImageViews{};

		VkCommandPool m_CmdPool = VK_NULL_HANDLE;

		std::vector<VkCommandBuffer> m_CmdBuffers{ k_MaxFramesInFlight };

		VkQueue m_GraphicsQueue = VK_NULL_HANDLE;
		VkQueue m_ComputeQueue = VK_NULL_HANDLE;
		VkQueue m_PresentQueue = VK_NULL_HANDLE;

		std::array<VkSemaphore, k_MaxFramesInFlight> m_ImageAvailableSemaphores{};
		std::vector<VkSemaphore> m_RenderFinishedSemaphores{};

		VkFence m_Fence = VK_NULL_HANDLE;


		// ===========================================================
		// INITIALIZATION HELPERS
		// ===========================================================

		void create_vk_instance( const RenderDeviceCreateInfo& info ) noexcept;
		void choose_adapter( ) noexcept;
		void create_logical_device( ) noexcept;
		void acquire_queues( ) noexcept;
		void create_main_surface( ) noexcept;
		void recreate_swapchain( ) noexcept;
		void recreate_swapchain_images( ) noexcept;
		void create_main_pipeline( ) noexcept;
		void create_command_pool( ) noexcept;
		void allocate_command_buffers( ) noexcept;
		void create_sync_primitives( ) noexcept;
		void handle_resize( ) noexcept;
		void create_vertex_buffers( ) noexcept;
		void bind_commands_implementations( ) noexcept;


		// ===========================================================
		// RUNTIME HELPERS
		// ===========================================================

		void assert_device( ) const;
		bool create_shader_module( const Path& path, VkShaderModule& module ) const noexcept;
		void decode_command_buffer( std::span<uint8> rawBuffer, VkCommandBuffer& vkBuffer ) noexcept;
		void process_commands( ) noexcept;


	};

} // namespace lum::rhi::vk