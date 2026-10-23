/*******************************************************/
/* /                                                 \ */
/*|-------------------|LumEngine|---------------------|*/
/*|            Cross-Platform Game Engine             |*/
/*|===================================================|*/
/*|                    GitHub:                        |*/
/*|       https://github.com/3zymek/LumEngine         |*/
/*|                                                   |*/
/*|        Copyright (c) 2025-present 3zymek          |*/
/*|          Licensed under the MIT License           |*/
/* \                                                 / */
/*******************************************************/
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
#include "Rhi/Backends/Vulkan/Core/RhiResourceSet_vulkan.hpp"
#include "Rhi/Backends/Vulkan/Core/RhiSampler_vulkan.hpp"
#include "Rhi/Backends/Vulkan/RhiTypes_vulkan.hpp"
#include "Rhi/Backends/Vulkan/Utils/CommandProcessor.hpp"
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
		void DestroyBuffer( const BufferHandle handle ) noexcept override;

		PipelineHandle CreatePipeline( const PipelineCreateInfo& info ) noexcept override;
		void DestroyPipeline( const PipelineHandle handle ) noexcept override;

		ImageHandle CreateImage( const ImageCreateInfo& info ) noexcept override;
		void DestroyImage( const ImageHandle handle ) noexcept override;

		SamplerHandle CreateSampler( const SamplerCreateInfo& info ) noexcept override;
		void DestroySampler( const SamplerHandle handle ) noexcept override;

		ResourceSetHandle CreateResourceSet( const ResourceSetCreateInfo& info ) noexcept override;
		void DestroyResourceSet( const ResourceSetHandle handle ) noexcept override;

		void SendCommandBuffer( CommandBuffer&& buffer ) noexcept override;

	private:

		static inline constexpr usize s_MaxBuffers		= 256;
		static inline constexpr usize s_MaxPipelines	= 32;
		static inline constexpr usize s_MaxImages		= 128;
		static inline constexpr usize s_MaxSamplers		= 32;
		static inline constexpr usize s_MaxResourceSets = 128;

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

		HandlePool<
			ResourceSetHandle,
			VulkanResourceSet,
			ResourceSetID> m_ResourceSets{ s_MaxResourceSets };


		VulkanCommandProcessor m_CommandProcessor{};
		std::queue<CommandBuffer> m_CommandBuffersToFlush{};

		Vector2u m_WindowSize{};

		VulkanAdapterEvaluator m_AdapterEvaluator{};
		VulkanAdapter m_Adapter{};

		VkExtent2D m_SwapchainExtent{};
		SafePtr<IVulkanSurfaceProvider> m_SurfaceProvider = nullptr;

		uint32 m_CurrentFrame = 0;

		bool m_IsInitialized = false;




		// ===========================================================
		// DEBUG
		// ===========================================================

		VkDescriptorSetLayout DT_DescriptorSetLayout{};
		VkDescriptorPool DT_DescriptorPool{};
		VkDescriptorSet DT_DescriptorSet{};



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
		void create_command_pool( ) noexcept;
		void allocate_command_buffers( ) noexcept;
		void create_sync_primitives( ) noexcept;
		void handle_resize( ) noexcept;
		void bind_commands_implementations( ) noexcept;


		// ===========================================================
		// RUNTIME HELPERS
		// ===========================================================

		void assert_device( ) const;
		bool create_shader_module( const Path& path, VkShaderModule& module ) const noexcept;
		void decode_command_buffer( std::span<uint8> rawBuffer, VkCommandBuffer& vkBuffer ) noexcept;
		void process_commands( VkCommandBuffer vkCmdBuffer ) noexcept;


	};

} // namespace lum::rhi::vk