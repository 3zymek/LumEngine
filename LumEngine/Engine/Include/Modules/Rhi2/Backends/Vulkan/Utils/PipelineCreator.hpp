#pragma once
#include "Rhi2/RhiCommon.hpp"
#include "Rhi2/Backends/Vulkan/Core/RhiPipeline_vulkan.hpp"

namespace lum::rhi::vk {

	class VulkanAdapter;

	class PipelineCreator {
	public:

		void Initialize( const VkDevice logicalDevice, VulkanAdapter& adapter ) {
			m_LogicalDevice = logicalDevice;
			m_Adapter = adapter;
		}

		Result<VulkanPipeline> CreatePipeline( const PipelineCreateInfo2& info );

	private:

		VkDevice m_LogicalDevice = VK_NULL_HANDLE;
		SafePtr<VulkanAdapter> m_Adapter = nullptr;

		// Internal PipelineCreator helpers
		auto setup_rasterizer_info( const RasterizationPass& info ) noexcept -> VkPipelineRasterizationStateCreateInfo;
		auto setup_color_blend_info( const ColorBlendPass& info ) noexcept -> VkPipelineColorBlendStateCreateInfo;
		auto setup_multisample_info( const MultisamplePass& info ) noexcept -> VkPipelineMultisampleStateCreateInfo;
		auto setup_assembly_info( const AssemblyPass& info ) noexcept -> VkPipelineInputAssemblyStateCreateInfo;
		auto setup_vertex_input_info( const VertexInputPass& info ) noexcept -> VkPipelineVertexInputStateCreateInfo;
		auto setup_shader_modules( const PipelineCreateInfo2& info ) noexcept -> std::vector<VkPipelineShaderStageCreateInfo>;
		auto create_shader_module( const Path& path ) noexcept -> VkShaderModule;

	};

} // namespace lum::rhi::vk