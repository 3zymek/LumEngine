#pragma once

#include <vector>

#include <vulkan/vulkan_core.h>

#include "Core/Utils/Result.hpp"
#include "Rhi/Backends/Vulkan/Core/RhiPipeline_vulkan.hpp"

namespace lum {

	class Path;

} // namespace lum

namespace lum::rhi {

	struct PipelineCreateInfo2;
	class RasterizationPass;
	class ColorBlendPass;
	class MultisamplePass;
	class AssemblyPass;
	class VertexInputPass;

} // namespace lum::rhi

namespace lum::rhi::vk {

	class VulkanAdapter;

	class PipelineCreator {
	public:

		Result<VulkanPipeline> CreatePipeline( const VkDevice logicalDevice, const VulkanAdapter& adapter, const PipelineCreateInfo2& info ) const noexcept;

	private:

		// Internal PipelineCreator helpers
		auto setup_rasterizer_info( const RasterizationPass& info ) const noexcept -> VkPipelineRasterizationStateCreateInfo;
		auto setup_color_blend_info( const ColorBlendPass& info ) const noexcept -> VkPipelineColorBlendStateCreateInfo;
		auto setup_multisample_info( const MultisamplePass& info ) const noexcept -> VkPipelineMultisampleStateCreateInfo;
		auto setup_assembly_info( const AssemblyPass& info ) const noexcept -> VkPipelineInputAssemblyStateCreateInfo;
		auto setup_vertex_input_info( const VertexInputPass& info ) const noexcept -> VkPipelineVertexInputStateCreateInfo;
		auto setup_shader_modules( const VkDevice logicalDevice, const PipelineCreateInfo2& info ) const noexcept -> std::vector<VkPipelineShaderStageCreateInfo>;
		auto create_shader_module( const VkDevice logicalDevice, const Path& path ) const noexcept -> VkShaderModule;

	};

} // namespace lum::rhi::vk