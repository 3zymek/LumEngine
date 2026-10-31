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

#include <vector>

#include <vulkan/vulkan_core.h>

#include "Core/Utils/Result.hpp"
#include "Rhi/Backends/Vulkan/Core/RhiPipeline_vulkan.hpp"

namespace lum {

	class Path;

} // namespace lum

namespace lum::rhi {

	struct PipelineCreateInfo;
	class RasterizationPass;
	class ColorBlendPass;
	class MultisamplePass;
	class AssemblyPass;
	class VertexInputPass;
	class PipelineLayout;

} // namespace lum::rhi

namespace lum::rhi::vk {

	class VulkanAdapter;
	class VulkanDevice;

	class PipelineFactory {
	public:

		Result<VulkanPipeline> CreatePipeline( VulkanDevice& device, const PipelineCreateInfo& info ) const noexcept;

	private:

		// Internal PipelineCreator helpers
		auto setup_rasterizer_info( const RasterizationPass& info ) const noexcept -> VkPipelineRasterizationStateCreateInfo;
		auto setup_color_blend_info( const ColorBlendPass& info ) const noexcept -> VkPipelineColorBlendStateCreateInfo;
		auto setup_multisample_info( const MultisamplePass& info ) const noexcept -> VkPipelineMultisampleStateCreateInfo;
		auto setup_assembly_info( const AssemblyPass& info ) const noexcept -> VkPipelineInputAssemblyStateCreateInfo;
		auto setup_vertex_input_info( const VertexInputPass& info ) const noexcept -> VkPipelineVertexInputStateCreateInfo;
		auto setup_pipeline_layout( const PipelineLayout& info, VulkanDevice& device ) const noexcept -> VkPipelineLayout;
		auto setup_shader_modules( const VkDevice logicalDevice, const PipelineCreateInfo& info ) const noexcept -> std::vector<VkPipelineShaderStageCreateInfo>;
		auto create_shader_module( const VkDevice logicalDevice, const Path& path ) const noexcept -> VkShaderModule;

	};

} // namespace lum::rhi::vk